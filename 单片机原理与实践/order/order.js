//index.js
//获取应用实例
const app = getApp()

Page({
  data: {
    // 统计商品数量和价格
    orderCount: {
      num: 0,
      money: 0
    },
    bottomFlag: false,
    items: []
  },
  // 点击对应菜单添加按钮
  del: function (event) {
    let that = this;
    let id = event.target.dataset.id;
    let index = event.target.dataset.index;
    let param = this.data.items[index]; //获取当前菜品

    if (param.num > 0) {
      param.num--; // 每点一次减少1
      // 改变添加按钮的状态
      this.data.items.splice(index, 1, param);
    }
    if (param.num == 0) { // 最低为0
      this.data.items.splice(index, 1); //当数量为0时从列表里面去除该菜品
    }

    that.setData({
      items: this.data.items //重新刷新列表
    });
    //更新本地缓存
    wx.setStorage({
      key: "orders",
      data: this.data.items
    });

    if (this.data.items.length == 0) {
      //当列表数据为0时自动跳转到首页
      wx.reLaunch({
        url: '../index/index'
      });
    }
    let money = 0;
    let num = 0;
    // 将已经确定总价格和数量求和
    this.data.items.forEach(item => {
      money += item.price * item.num; // 总价格求和
      num += item.num; //总件数
    });
    let orderCount = {
      num,
      money
    }
    // 设置显示对应的总数和全部价钱
    this.setData({
      orderCount
    });
  },
  // 点击对应菜单添加按钮
  add: function (event) {
    let that = this;
    let id = event.target.dataset.id;
    let index = event.target.dataset.index;
    let param = this.data.items[index];
    param.num++; // 每点一次增加1
    // 改变添加按钮的状态
    this.data.items.splice(index, 1, param);
    that.setData({
      items: this.data.items //更新列表
    });
    //更新缓存
    wx.setStorage({
      key: "orders",
      data: this.data.items
    });
    let money = 0;
    let num = 0;
    // 将已经确定总价格和数量求和
    this.data.items.forEach(item => {
      money += item.price * item.num;
      num += item.num;
    });
    let orderCount = {
      num,
      money
    }
    // 设置显示对应的总数和全部价钱
    this.setData({
      orderCount
    });
  },
  // 点击结账按钮
  pay: function () {
    let that = this;
    let str = '选中' + that.data.orderCount.num + '件商品，共' + that.data.orderCount.money + '元，是否要支付？'
    // 至少选中一个商品才能支付
    if (that.data.orderCount.num !== 0) {
      wx.showModal({
        title: '提示',
        content: str,
        success: function (res) {

          if (res.confirm) {
            // 打开扫码功能
            wx.scanCode({
              onlyFromCamera: true,
              success: (res) => {
                wx.redirectTo({
                  url: '../pay/pay'
                });
              }
            });
          } else if (res.cancel) {
            console.log('用户点击取消')
          }

        }
      })
    } else {
      wx.showToast({
        title: '您未选中任何商品',
        icon: 'none',
        duration: 2000
      })
    }
  },
  //清空购物车
  clearItems: function () {
    let that = this;
    wx.setStorage({
      key: 'orders',
      data: [],
    })
    this.setData({
      items: []
    })
    let money = 0;
    let num = 0;
    let orderCount = {
      num,
      money
    }
    // 设置显示对应的总数和全部价钱
    that.setData({
      orderCount
    });
    //当列表数据为0时自动跳转到首页
    /*wx.reLaunch({
      url: '../index/index'
    });*/
  },
  onShow: function () {
    let that = this;
    // 取出订单传过来的数据
    wx.getStorage({
      key: 'orders',
      success: function (res) {
        that.setData({
          items: res.data
        });
        // 价格统计汇总
        let money = 0;
        let num = 0;
        res.data.forEach(item => {
          money += item.price * item.num; // 总价格求和
          num += item.num; //总件数
        });
        let orderCount = {
          num,
          money
        }
        // 设置显示对应的总数和全部价钱
        that.setData({
          orderCount
        });
      }
    })
  }
})
