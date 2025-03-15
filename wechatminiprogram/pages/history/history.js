Page({
  data: {
    historyData: [],  // 存储心率和血氧的历史数据
    loading: true,    // 控制加载状态
    error: null       // 存储错误信息
  },
  onLoad: function () {
    this.fetchHistoryData();
    setInterval(() => this.fetchHistoryData(), 3000); // 每3秒刷新一次
  },
  fetchHistoryData: function () {
    wx.request({
      url: 'https://iot-api.heclouds.com/datapoint/history-datapoints',
      method: 'GET',
      header: {
        'Accept': 'application/json, text/plain, */*',
        'authorization': 'version=2022-05-01&res=userid%2F426691&et=1745638586&method=sha1&sign=5kWg%2B8w3cAUCTeLG5yPc4ZOQ5Ro%3D'
      },
      data: {
        product_id: '3VQ6EZjF8b',
        device_name: 'ESP8266'
      },
      success: (res) => {
        console.log('获取成功', res);
        if (res.data.code === 0) {
          const allDatastreams = res.data.data.datastreams;
          const filteredDatastreams = allDatastreams.filter(stream => 
            stream.id === 'dis_hr' || stream.id === 'dis_spo2'
          );
          const hrStream = filteredDatastreams.find(stream => stream.id === 'dis_hr');
          const spo2Stream = filteredDatastreams.find(stream => stream.id === 'dis_spo2');
          
          // 获取心率和血氧的 datapoints
          const hrDatapoints = hrStream ? hrStream.datapoints : [];
          const spo2Datapoints = spo2Stream ? spo2Stream.datapoints : [];
          
          // 取最新的数据点（假设最新的在数组最后）
          const latestHr = hrDatapoints[hrDatapoints.length - 1];
          const latestSpo2 = spo2Datapoints[spo2Datapoints.length - 1];
          
          if (latestHr && latestSpo2 && latestHr.at === latestSpo2.at) {
            const newData = {
              hr: latestHr.value,
              spo2: latestSpo2.value,
              at: latestHr.at
            };
            
            // 获取当前 historyData 的最新记录
            const currentLatest = this.data.historyData[0];
            
            // 检查新数据是否与最新记录相同
            if (!currentLatest || newData.hr !== currentLatest.hr || newData.spo2 !== currentLatest.spo2) {
              // 如果不同，则将新数据添加到顶部
              let updatedHistoryData = [newData, ...this.data.historyData];
              
              // 如果超过30条，删除最旧的数据（即数组末尾）
              if (updatedHistoryData.length > 30) {
                updatedHistoryData = updatedHistoryData.slice(0, 30);
              }
              
              this.setData({
                historyData: updatedHistoryData,
                loading: false
              });
            } else {
              // 如果相同，不添加新数据，仅更新加载状态
              console.log('数据与最新记录相同，不添加');
              this.setData({
                loading: false
              });
            }
          } else {
            this.setData({
              error: '心率和血氧数据时间戳不匹配',
              loading: false
            });
          }
        } else {
          this.setData({
            error: res.data.msg,
            loading: false
          });
        }
      },
      fail: (err) => {
        this.setData({
          error: '网络请求失败',
          loading: false
        });
      }
    });
  }
});