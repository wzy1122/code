// miniprogram/pages/find_delete.js
const app = getApp()
var util = require('../../util.js');
const db = wx.cloud.database({ env: 'testsduwh030201-icc8o'});
Page({

  /**
   * 页面的初始数据
   */
  data: {
    time:"",
    num:0,
    cur:0,
    unit:"",
    main_index:"",
    extra_index:"",
    description:"",
    body:"",
    answer:"",
    comment:"",
    tasks:{},
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    let that=this
    this.onGetOpenid()
    var DATE = util.formatDate(new Date());
    this.setData({
      time:DATE,
    })
    this._init()
  },
  _init:function(){
    this.get_num().then((data)=>{
      console.log(data)
      this.setData({
        num: data,
        cur: data-1
      })
      console.log("num",this.data.num)
    })
    setTimeout(()=>{this.on_tap()},1000)
    setTimeout(()=>{this._add()},2000)
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
  get_num:function() {
    var promise = new Promise((resolve, reject)=>{
      db.collection('Advanced_English').where({
        _openid: app.globalData.openid,
        time: this.data.time
      }).count({
        success: res=>{
          resolve(res.total)
        },
        error: e=>{
          reject("fail")
        }
      })
    })
    return promise
  },
  _get:function(){
    const batchTimes = Math.ceil(this.data.num / 20)
    console.log("batchTimes",batchTimes)
    const task=[]
      for(let i=0;i<batchTimes;i++){
        var promise = new Promise((resolve, reject)=>{
        db.collection('Advanced_English').where({
          _openid: app.globalData.openid,
          time: this.data.time
        }).skip(i*20).limit(20).get({
          success: res=>{
            resolve(res.data)
          },
          error: e=>{
            reject("fail")
          }
        })
      })
      task.push(promise)
    }
     return Promise.all(task)
  },
  on_tap:function(){
    let that=this
    that._get().then((data)=>{
      let len = data.length
      let t=[]
      console.log(len)
      for(let i=0;i<len;i++){
        for(let j=0;j<data[i].length;j++){
          t.push(data[i][j])
        }
      }
        console.log(t)
       this.setData({
         tasks: t
       })
    })
  },
  _add:function(){
    let cur=this.data.cur
    // console.log(this.data.tasks[cur]._id)
    this.setData({
      unit:this.data.tasks[cur].unit,
      main_index:this.data.tasks[cur].main_index,
      extra_index:this.data.tasks[cur].extra_index,
      description:this.data.tasks[cur].description,
      body:this.data.tasks[cur].body,
      answer:this.data.tasks[cur].answer,
      comment:this.data.tasks[cur].comment,
    })
  },
  _previous:function(){
    let cur=this.data.cur
    if(cur==0){
      wx.showModal({
        title: '没有上一项！',
      })
    }else{
      cur--
      console.log("cur",cur)
      this.setData({
        cur:cur,
        unit:this.data.tasks[cur].unit,
        main_index:this.data.tasks[cur].main_index,
        extra_index:this.data.tasks[cur].extra_index,
        description:this.data.tasks[cur].description,
        body:this.data.tasks[cur].body,
        answer:this.data.tasks[cur].answer,
        comment:this.data.tasks[cur].comment,
    })
    }
  },
  _next:function(){
    let cur=this.data.cur
    if(cur==this.data.num-1){
      wx.showModal({
        title: '没有下一项！',
      })
    }else{
      cur++
      console.log("cur",cur)
      this.setData({
        cur:cur,
        unit:this.data.tasks[cur].unit,
        main_index:this.data.tasks[cur].main_index,
        extra_index:this.data.tasks[cur].extra_index,
        description:this.data.tasks[cur].description,
        body:this.data.tasks[cur].body,
        answer:this.data.tasks[cur].answer,
        comment:this.data.tasks[cur].comment,
    })
    }
  },
  _modify:function(){
    let cur=this.data.cur
    let id=this.data.tasks[cur]._id
    console.log("id",id)
    db.collection('Advanced_English').doc(id).update({
      data:{
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
          title: '修改记录成功！',
        })
      }
    })
    setTimeout(()=>{
      this.get_num().then((data)=>{
        console.log(data)
        this.setData({
          num: data,
        })
        console.log("num",this.data.num)
      })
      setTimeout(()=>{this.on_tap()},1000)
      setTimeout(()=>{this._add()},2000)
    },100)
  },
  _delete:function(){
    let cur=this.data.cur
    let id=this.data.tasks[cur]._id
    // console.log("id",id)
    db.collection('Advanced_English').doc(id).remove({
      success: res=>{
        wx.showToast({
          title: '删除成功！',
        })
      },
      error: e=>{
        wx.showToast({
          title: '删除失败！',
        })
      }
    })
    if(cur==(this.data.num-1)) cur--
    setTimeout(()=>{
      this.get_num().then((data)=>{
        console.log(data)
        this.setData({
          num: data,
          cur: cur
        })
        console.log("num",this.data.num)
      })
      setTimeout(()=>{this.on_tap()},1000)
      setTimeout(()=>{this._add()},2000)
    },100)
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
})
