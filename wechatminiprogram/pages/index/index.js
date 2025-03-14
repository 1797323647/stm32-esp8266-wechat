Page({
  data: {
    heartRate: '---',      // 心率
    spo2: '---',         // 血氧
    ax: '---',           // 加速度X轴
    ay: '---',           // 加速度Y轴
    az: '---',           // 加速度Z轴
    gx: '---',           // 陀螺仪X轴
    gy: '---',           // 陀螺仪Y轴
    gz: '---',           // 陀螺仪Z轴
    activityStatus: '静止', // 活动状态，默认为“静止”
    prevAx: 0,          // 上一次的加速度X轴值，初始值为 0
    prevAy: 0,          // 上一次的加速度Y轴值，初始值为 0
    prevAz: 0,          // 上一次的加速度Z轴值，初始值为 0
    prevGx: 0,          // 上一次的陀螺仪X轴值，初始值为 0
    prevGy: 0,          // 上一次的陀螺仪Y轴值，初始值为 0
    prevGz: 0,         // 上一次的陀螺仪Z轴值，初始值为 0
    question: '',       // 用户输入的问题
    aiAnswer: '',       // AI 的回答
    isThinking: false,  // 是否正在思考
    isDeepThinking: false  // 新增：控制深度思考按钮状态
  },

  onLoad: function () {
    this.fetchData(); // 页面加载时立即请求一次数据
    setInterval(this.fetchData, 3000); // 每 3 秒定时刷新数据
  },

  toggleDeepThinking: function() {
    this.setData({
      isDeepThinking: !this.data.isDeepThinking
    });
  },

  inputQuestion: function(e) {
    this.setData({
      question: e.detail.value
    });
  },

  askAI: function() {
    const that = this;
    if (!that.data.question) {
      wx.showToast({ title: '问问deepseek吧', icon: 'none' });
      return;
    }
    // 设置正在思考状态，清空之前的回答
    that.setData({
      isThinking: true,
      aiAnswer: ''
    });
    
    // 根据 isDeepThinking 选择模型
    const model = that.data.isDeepThinking ? 'deepseek-reasoner' : 'deepseek-chat';
    
    wx.request({
      url: 'https://api.deepseek.com/chat/completions',
      method: 'POST',
      header: {
        'Content-Type': 'application/json',
        'Authorization': 'Bearer sk-0c3d0d73693a477eaacb057dd56f3394' // 请确保使用真实的 API 密钥
      },
      data: {
        model: model,
        messages: [{ role: 'user', content: that.data.question }]
      },
      success: res => {
        if (res.data.choices && res.data.choices.length > 0) {
          that.setData({
            aiAnswer: res.data.choices[0].message.content + " By " + model + " - " + new Date().toLocaleString(),
            question: '', // 清空输入框
            isThinking: false
          });
        } else {
          that.setData({
            aiAnswer: 'AI 回答失败',
            isThinking: false
          });
          wx.showToast({ title: 'AI 回答失败', icon: 'none' });
        }
      },
      fail: err => {
        that.setData({
          aiAnswer: '网络请求失败',
          isThinking: false
        });
        wx.showToast({ title: '网络请求失败', icon: 'none' });
      }
    });
  },

  fetchData: function () {
    const that = this; // 保存 this 引用
    wx.request({
      url: 'http://iot-api.heclouds.com/datapoint/current-datapoints?product_id=3VQ6EZjF8b&device_name=ESP8266',
      method: 'GET',
      header: {
        'Authorization': 'version=2022-05-01&res=userid%2F426691&et=1741943453&method=sha1&sign=s7HLTGAVHhDhy0tpVEKZFyY9%2FLo%3D'
      },
      data: {
        'datastream_ids': 'dis_hr,dis_spo2,AX,AY,AZ,GX,GY,GZ'
      },
      success: res => {
        console.log('API 请求成功', res.data);
        
        if (res.data.code == 0) {
          let datastreams = res.data.data.devices[0].datastreams;
          
          let hrValue = '---';
          let spo2Value = '---';
          let axValue = 0;
          let ayValue = 0;
          let azValue = 0;
          let gxValue = 0;
          let gyValue = 0;
          let gzValue = 0;
          
          datastreams.forEach(item => {
            if (item.id === 'dis_hr') {
              hrValue = item.value;
            } else if (item.id === 'dis_spo2') {
              spo2Value = item.value;
            } else if (item.id === 'AX') {
              axValue = parseInt(item.value);
            } else if (item.id === 'AY') {
              ayValue = parseInt(item.value);
            } else if (item.id === 'AZ') {
              azValue = parseInt(item.value);
            } else if (item.id === 'GX') {
              gxValue = parseInt(item.value);
            } else if (item.id === 'GY') {
              gyValue = parseInt(item.value);
            } else if (item.id === 'GZ') {
              gzValue = parseInt(item.value);
            }
          });
          
          // 活动状态检测逻辑
          let activityStatus = '静止';
          const changeThresholdWalking = 400;
          const changeThresholdRunning = 1800;
          
          const accelerationChangeMagnitude = Math.abs(axValue - this.data.prevAx) + Math.abs(ayValue - this.data.prevAy) + Math.abs(azValue - this.data.prevAz);
          const gyroscopeChangeMagnitude = Math.abs(gxValue - this.data.prevGx) + Math.abs(gyValue - this.data.prevGy) + Math.abs(gzValue - this.data.prevGz);
          const totalChangeMagnitude = accelerationChangeMagnitude + gyroscopeChangeMagnitude;
          
          if (totalChangeMagnitude > changeThresholdRunning) {
            activityStatus = '跑步';
          } else if (totalChangeMagnitude > changeThresholdWalking) {
            activityStatus = '行走';
          } else {
            activityStatus = '静止';
          }
          
          // 更新数据
          this.setData({
            heartRate: hrValue,
            spo2: spo2Value,
            ax: axValue,
            ay: ayValue,
            az: azValue,
            gx: gxValue,
            gy: gyValue,
            gz: gzValue,
            activityStatus: activityStatus,
            prevAx: axValue,
            prevAy: ayValue,
            prevAz: azValue,
            prevGx: gxValue,
            prevGy: gyValue,
            prevGz: gzValue
          });
        } else {
          console.error('API 请求错误', res.data);
          wx.showToast({
            title: '数据获取失败',
            icon: 'none',
            duration: 2000
          });
        }
      },
      fail: err => {
        console.error('API 请求失败', err);
        wx.showToast({
          title: '网络请求失败',
          icon: 'none',
          duration: 2000
        });
      }
    });
  },
})