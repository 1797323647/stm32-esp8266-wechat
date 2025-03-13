Page({
  data: {
    history: []
  },

  onLoad: function() {
    // 从主页面传递历史记录
    const eventChannel = this.getOpenerEventChannel();
    eventChannel.on('sendHistory', (data) => {
      this.setData({
        history: data.history
      });
    });
  }
});