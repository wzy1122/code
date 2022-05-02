// miniprogram/pages/home.js
const app = getApp()
var util = require('../../util.js');
const db = wx.cloud.database({ env: 'testsduwh030201-icc8o'});
Page({

  /**
   * 页面的初始数据
   */
  data: {
    time:"",
    unit:"",
    main_index:"",
    extra_index:"",
    description:"",
    body:"",
    answer:"",
    comment:"",

  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.onGetOpenid()
    var DATE = util.formatDate(new Date());
    console.log(DATE)
    this.setData({
      time:DATE,
    })
  },

  onGetOpenid: function() {
    // 调用云函数
    wx.cloud.callFunction({
      name: 'login',
      data: {},

      success: res => {
        console.log('[云函数] [login] user openid: ', res.result.openid)
        app.globalData.openid = res.result.openid
      },
      fail: err => {
        console.error('[云函数] [login] 调用失败', err)
      }
    })
  },
  _get: function() {
    wx.navigateTo({
      url: '../find_delete/find_delete',
    })
  },
  input_unit:function(e){
    // console.log(e.detail.value)
    this.setData({unit: e.detail.value})
  },
  input_main_index:function(e){
    // console.log(e.detail.value)
    this.setData({main_index: e.detail.value})
  },
  input_extra_index:function(e){
    // console.log(e.detail.value)
    this.setData({extra_index: e.detail.value})
  },
  input_description:function(e){
    // console.log(e.detail.value)
    this.setData({description: e.detail.value})
  },
  input_body:function(e){
    // console.log(e.detail.value)
    this.setData({body: e.detail.value})
  },
  input_answer:function(e){
    // console.log(e.detail.value)
    this.setData({answer: e.detail.value})
  },
  input_comment:function(e){
    // console.log(e.detail.value)
    this.setData({comment: e.detail.value})
  },

  _add:function(){
    let that = this
    setTimeout(()=>{},10)
    db.collection('Advanced_English').add({
      data:{
        time:this.data.time,
        unit:this.data.unit,
        main_index:this.data.main_index,
        extra_index:this.data.extra_index,
        description:this.data.description,
        body:this.data.body,
        answer:this.data.answer,
        comment:this.data.comment,
      },
      success: res =>{
        wx.showToast({
          title: '新增记录成功',
        })
      }
    })
    that.setData({
      unit:"",
      main_index:"",
      extra_index:"",
      description:"",
      body:"",
      answer:"",
      comment:"",
    })
  }
})