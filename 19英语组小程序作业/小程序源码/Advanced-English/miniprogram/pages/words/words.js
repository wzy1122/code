
const app = getApp()
var util = require('../../util.js');
const db = wx.cloud.database({ env: 'testsduwh030201-icc8o'});
Page({

  /**
   * 页面的初始数据
   */
  data: {
    time:"",
    simple_description_num:1,
    simple_description:[],
    english_description_num:1,
    english_description:[],
    english_example:"",
    chinese_translation:"",
    answer:"",
    choice1:"",
    choice2:"",
    choice3:"",
    choice4:"",
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
      url: '../find_delete_words/find_delete_words',
    })
  },
  simple_description_num: function(e){
    let that=this;
    // console.log(e);
    let n = e.detail.value;
    that.data.simple_description = [];
    for(let i = 0;i < n;i++){
      that.data.simple_description.push({
        "text":"",
      })
    }
     this.setData({'simple_description':this.data.simple_description})
    this.setData({ simple_description_num: e.detail.value})
  },
  english_description_num: function(e){
    let that=this;
    // console.log(e);
    let n = e.detail.value;
    that.data.english_description = [];
    for(let i = 0;i < n;i++){
      that.data.english_description.push({
        "text":""
      })
    }
    this.setData({'english_description':this.data.english_description})
    this.setData({ english_description_num: e.detail.value})
  },
  input_simple_description: function(e){
    let that=this
    let num=e.currentTarget.dataset.no;
    let tmp = this.data.simple_description;
    // console.log(tmp)
    if(e.detail.value)
      tmp[num].text=e.detail.value;
     this.setData({simple_description:tmp})
  },
  input_english_description: function(e){
    let that=this
    let num=e.currentTarget.dataset.no;
    let tmp = this.data.english_description;
    // console.log(tmp)
    if(e.detail.value)
      tmp[num].text=e.detail.value;
     this.setData({english_description:tmp})
  },
  inputanswer:function(e){
    this.setData({answer:e.detail.value})
  },
  inputchoice1:function(e){
    this.setData({choice1:e.detail.value})
  },
  inputchoice2:function(e){
    this.setData({choice2:e.detail.value})
  },
  inputchoice3:function(e){
    this.setData({choice3:e.detail.value})
  },
  inputchoice4:function(e){
    this.setData({choice4:e.detail.value})
  },
  _add:function(){
    let that = this
    let tmp1=[];let tmp2=[];let tmp3=[]
    // console.log(this.data.simple_description[0].text)
    for(let i=0;i<that.data.simple_description_num;i++){
      // console.log(that.data.simple_description[i].text)
      if(that.data.simple_description[i].text)
        tmp1.push(that.data.simple_description[i].text)
    }
    console.log("tmp1:",tmp1)
    // console.log("english:",this.data.english_description_num)
    for(let i=0;i<that.data.english_description_num;i++){
      // console.log(that.data.english_description[i].text)
      if(that.data.english_description[i].text)
        tmp2.push(that.data.english_description[i].text)
    }
    console.log("tmp2:",tmp2)
    tmp3.push(this.data.answer,this.data.choice1,this.data.choice2,this.data.choice3,this.data.choice4)
    console.log("tmp3:",tmp3)
    setTimeout(()=>{
      db.collection('Words_CET_4').add({
         data:{
           time:this.data.time,
           english_example:this.data.english_example,
           chinese_translation:this.data.chinese_translation,
           simple_description_num:parseInt(this.data.simple_description_num),
           english_description_num:parseInt(this.data.english_description_num),
           simple_description:tmp1,
           english_description:tmp2,
           choices:tmp3,
         },
        success: res =>{
          wx.showToast({
            title: '新增记录成功',
          })
        }
      })
    },1000)
    // that.setData({
    //   simple_description_num:1,
    //   simple_description:[],
    //   english_description_num:1,
    //   english_description:[],
    //   english_example:"",
    //   chinese_translation:"",
    //   answer:"",
    //   choice1:"",
    //   choice2:"",
    //   choice3:"",
    //   choice4:"",
    // })
  },
})