Page({
  data: {
    url: ''
  },
  onLoad: function (options) {
    this.setData({
      url: options.url // 从导航参数中获取 URL
    });
  }
});