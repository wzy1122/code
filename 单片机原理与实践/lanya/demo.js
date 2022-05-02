//index.js
//获取应用实例
var app = getApp();
var receData = ""
var devicesFlag = false;
Page({
  data: {
    status: "",
    sousuo: "",
    connectedDeviceId: "", //已连接设备uuid
    services: "", // 连接设备的服务
    characteristics: "",   // 连接设备的状态值
    writeServicweId: "", // 可写服务uuid
    writeCharacteristicsId: "",//可写特征值uuid
    readServicweId: "", // 可读服务uuid
    readCharacteristicsId: "",//可读特征值uuid
    notifyServicweId: "", //通知服务UUid
    notifyCharacteristicsId: "", //通知特征值UUID
    inputValue: "",
    characteristics1: "", // 连接设备的状态值
    index: 0,
    ALLUUID: "0000FEE9-0000-XXXX-1900-00AAAAAAAAFB" //同时具有可读、可写、通知三种属性的UUID
  },
  onLoad: function () {
    if (wx.openBluetoothAdapter) {
      wx.openBluetoothAdapter()
    } else {
      // 如果希望用户在最新版本的客户端上体验您的小程序，可以这样子提示
      wx.showModal({
        title: '提示',
        content: '当前微信版本过低，无法使用该功能，请升级到最新微信版本后重试。'
      })
    }

    //版本
    wx.getSystemInfo({
      success: function (res) {
        var version = res.SDKVersion
        version = version.replace(/\./g, "")
        console.log("sdk版本：" + version)
      },
    })

    //搜索设备
    var that = this
    searchDevice(that)

  },
  //连接设备
  connectTO: function (e) {
    showLoading('连接中...')
    var that = this;
    wx.createBLEConnection({
      deviceId: e.currentTarget.id,
      success: function (res) {
        console.log(res.errMsg);
        that.setData({
          connectedDeviceId: e.currentTarget.id,
          msg: "已连接" + e.currentTarget.id,
          msg1: "",
          devicesFlag: false

        })
        showLoading('连接成功')
        setTimeout(function () {
          wx.hideLoading()
        }, 2000)

        //5、停止搜索设备
        stopBluetoothDevicesDiscovery()
        //6、 获取连接设备的service服务
        wx.getBLEDeviceServices({
          // 这里的 deviceId 需要在上面的 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
          deviceId: that.data.connectedDeviceId,
          success: function (res) {
            console.log('device services:', JSON.stringify(res.services));
            that.setData({
              services: res.services,
              msg: JSON.stringify(res.services),
            })

            //7、获取连接设备的所有特征值  for循环获取不到值 start
            wx.getBLEDeviceCharacteristics({
              // 这里的 deviceId 需要在上面的 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
              deviceId: that.data.connectedDeviceId,
              // 这里的 serviceId 需要在上面的 getBLEDeviceServices 接口中获取
              serviceId: that.data.ALLUUID,
              success: function (res) {
                for (var i = 0; i < res.characteristics.length; i++) {


                  if (res.characteristics[i].properties.notify) {
                    console.log("获取开启notify的ServicweId：", that.data.ALLUUID);
                    console.log("获取开启notify的CharacteristicsId：", res.characteristics[i].uuid);
                    that.setData({
                      notifyServicweId: that.data.ALLUUID,
                      notifyCharacteristicsId: res.characteristics[i].uuid,

                    })
                  }
                  if (res.characteristics[i].properties.write) {
                    console.log("获取开启write的ServicweId：", that.data.ALLUUID);
                    console.log("获取开启write的CharacteristicsId：", res.characteristics[i].uuid);
                    that.setData({
                      writeServicweId: that.data.ALLUUID,
                      writeCharacteristicsId: res.characteristics[i].uuid,
                    })

                  } else if (res.characteristics[i].properties.read) {
                    console.log("读取函数：", that.data.ALLUUID);
                    console.log("读取函数：", res.characteristics[i].uuid);
                    that.setData({
                      readServicweId: that.data.ALLUUID,
                      readCharacteristicsId: res.characteristics[i].uuid,
                    })

                  }

                }
                console.log('device getBLEDeviceCharacteristics:', res.characteristics);

                that.setData({
                  msg: JSON.stringify(res.characteristics),
                })
                ////8、启用低功耗蓝牙设备特征值变化时的 notify 功能
                var notifyServicweId = that.data.notifyServicweId.toUpperCase();
                var notifyCharacteristicsId = that.data.notifyCharacteristicsId.toUpperCase();
                console.log("启用notify的serviceId", notifyServicweId);
                console.log("启用notify的notifyCharacteristicsId", notifyCharacteristicsId);

                wx.notifyBLECharacteristicValueChange({
                  state: true, // 启用 notify 功能
                  type: 'notification',
                  // 这里的 deviceId 需要在上面的 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取
                  deviceId: that.data.connectedDeviceId,
                  // 这里的 serviceId 需要在上面的 getBLEDeviceServices 接口中获取
                  serviceId: that.data.notifyServicweId,

                  // 这里的 characteristicId 需要在上面的 getBLEDeviceCharacteristics 接口中获取
                  characteristicId: that.data.notifyCharacteristicsId,

                  success: function (res) {
                    console.log('66666启用通知notifyBLECharacteristicValueChange success', res.errMsg)

                    setTimeout(function () {
                      //9、接收函数
                      onBLECharacteristicValueChange(that)
                    }, 1000)


                  },
                  fail: function () {
                    console.log('shibai0');
                    console.log(that.data.notifyServicweId);
                    console.log(that.data.notifyCharacteristicsId);
                  },
                })



              },
              fail: function () {
                console.log("fail");
              },
              complete: function () {
                console.log("complete");
              }
            })
            //7、获取连接设备的所有特征值  for循环获取不到值 end


          }



        })

      },
      fail: function () {
        console.log("调用失败");
      },
      complete: function () {
        console.log("调用结束");
      }

    })
    console.log(that.data.connectedDeviceId);
  },
  //断开设备连接
  lanya0: function () {
    var that = this;
    wx.closeBLEConnection({
      deviceId: that.data.connectedDeviceId,
      success: function (res) {
        that.setData({
          connectedDeviceId: "",
        })
      }
    })
  },
  //监听input表单
  inputTextchange: function (e) {
    this.setData({
      inputValue: e.detail.value
    })
  },

  //向蓝牙设备发送一个0x00的16进制数据
  lanya8: function () {
    var that = this
    console.log('开始发送指令------------')
    that.setData({
      receData: ''
    })
    receData = ''
    var hex = that.data.comandInput
    console.log('发送的指令' + hex)
    that.setData({
      sendData: '发送指令:' + hex

    })

    //开始分包

    var dataArray = datasubpck(hex)
    console.log(that.data.connectedDeviceId + "分包后的数组" + dataArray)
    console.log(that.data.connectedDeviceId)
    console.log(that.data.writeServicweId)
    console.log(that.data.writeCharacteristicsId)
    deviceWrite(that.data.connectedDeviceId, that.data.writeServicweId, that.data.writeCharacteristicsId, dataArray, 0, that)


  },

  comandInput: function (e) {
    var that = this
    console.log('指令------' + e.detail.value)
    that.setData({
      comandInput: e.detail.value
    })
  }


})

//www.vxzsk.com V型知识库原创，请尊重版权
// ArrayBuffer转16进度字符串示例
function ab2hex(buffer) {
  var hexArr = Array.prototype.map.call(
    new Uint8Array(buffer),
    function (bit) {
      return ('00' + bit.toString(16)).slice(-2)
    }
  )
  return hexArr.join('');
}

//分包方法 linfanhe 20180524
function datasubpck(datahex) {
  var arrayObj = new Array();
  for (var i = 0; i < datahex.length; i += 40) {
    // 预加 最大包长度，如果依然小于总数据长度，可以取最大包数据大小
    if ((i + 40) < datahex.length) {
      arrayObj.push(datahex.substring(i, i + 40))

    } else {
      arrayObj.push(datahex.substring(i))
    }
  }
  return arrayObj;
}

function sleep(numberMillis) {
  console.log("睡眠" + numberMillis + "毫秒")
  var now = new Date();
  var exitTime = now.getTime() + numberMillis;
  while (true) {
    now = new Date();
    if (now.getTime() > exitTime)
      return;
  }
}

var totalLength = 0
function deviceWrite(connectedDeviceId, writeServicweId, writeCharacteristicsId, dataArray, index1, that) {

  console.log('++++++++++++++++++')
  totalLength = dataArray.length
  console.log("index:::" + index1)
  var subhex = ''
  subhex = dataArray[index1]

  var temp = subhex;//'0600031567890302'
  var prefix = '0x'
  var affix = ','
  var temp2 = ''
  for (var i = 0; i < temp.length / 2; i++) {
    if (i < (temp.length / 2 - 1)) {
      temp2 = temp2 + prefix + temp.substring(i * 2, i * 2 + 2) + affix
    }
    else {
      temp2 = temp2 + prefix + temp.substring(i * 2)
    }
    console.log("i:" + i + "temp2:" + temp2)
  }


  //subhex = subhex.toLowerCase()
  console.log('钱钱钱钱---' + subhex)
  var buffer1 = new ArrayBuffer(subhex.length / 2)//字节长度 一个字节两个字符所以除以2
  var dataView1 = new DataView(buffer1)
  console.log('++++++++++++++++++3')
  var str1 = temp2;
  console.log('aaaaaa啊啊啊' + str1)
  var arr1 = str1.split(',')
  var val;
  for (var i = 0; i < arr1.length; i++) {
    val = parseInt(arr1[i], 16)
    dataView1.setInt8(i, val);
  }
  console.log('++++++++++++++++++4')

  console.log("bbbsubhex:::" + subhex)
  index1 = index1 + 1
  var typedArray = new Uint8Array(subhex.match(/[\da-f]{2}/gi).map(function (h) {
    return parseInt(h, 16)
  }))
  console.log(typedArray)
  console.log("发送的指令===" + ab2hex(buffer1))
  console.log("发送指令函数deviceId：" + connectedDeviceId)
  console.log("发送指令函数serviceId：" + writeServicweId)
  console.log("发送指令函数CharacteristicsId：" + writeCharacteristicsId)

  wx.writeBLECharacteristicValue({
    // 这里的 deviceId 需要在上面的 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取  
    deviceId: connectedDeviceId,
    // 这里的 serviceId 需要在上面的 getBLEDeviceServices 接口中获取  
    serviceId: writeServicweId,
    // 这里的 characteristicId 需要在上面的 getBLEDeviceCharacteristics 接口中获取  
    characteristicId: writeCharacteristicsId,
    value: buffer1,
    success: function (res) {
      //sleep(200)
      console.log('writeBLECharacteristicValue success', res.errMsg)
      setTimeout(function () {

        if (index1 < totalLength) {
          deviceWrite(connectedDeviceId, writeServicweId, writeCharacteristicsId, dataArray, index1, that);

        } else {
          console.log("stop command" + index1)
        }

      }, 40)


    },
    fail: function (res) {
      console.log("写入失败");

    }
  })

}


//www.vxzsk.com V型知识库原创，请尊重版权

//搜索设备 start
function searchDevice(that) {
  showLoading('搜索中')


  wx.openBluetoothAdapter({
    success: function (res) {
      console.log("初始化蓝牙适配器成功")
      that.setData({
        msg: "初始化蓝牙适配器成功！" + JSON.stringify(res),
      })
      // 本机蓝牙适配器状态
      wx.getBluetoothAdapterState({
        success: function (res) {
          that.setData({
            msg: "本机蓝牙适配器状态" + "/" + JSON.stringify(res.errMsg),
            sousuo: res.discovering ? "在搜索。" : "未搜索。",
            status: res.available ? "可用。" : "不可用。",
          })

        }
      })

      //开始搜索设备 start----
      wx.startBluetoothDevicesDiscovery({
        //services: ['FEE7'],
        //services: ['F2E0'],
        allowDuplicatesKey: false,
        success: function (res) {
          console.log("搜索设备啊" + JSON.stringify(res))
          that.setData({
            msg: "搜索设备" + JSON.stringify(res),
          })
          //监听蓝牙适配器状态
          wx.onBluetoothAdapterStateChange(function (res) {
            that.setData({
              sousuo: res.discovering ? "在搜索。" : "未搜索。",
              status: res.available ? "可用。" : "不可用。",
            })
          })

          //获取所有已发现的设备 start 
          setTimeout(function () {

            wx.getBluetoothDevices({
              success: function (res) {
                //是否有已连接设备
                wx.getConnectedBluetoothDevices({
                  success: function (res) {
                    console.log('被连接的设备' + JSON.stringify(res.devices));
                    that.setData({
                      connectedDeviceId: res.deviceId
                    })


                  },
                  fail: function () {
                    console.log('无连接设备-------------------')
                  }

                })

                console.log('搜到的蓝牙设备数目：' + res.devices.length)
                console.log('获取到周边搜到的设备信息：' + JSON.stringify(res.devices))
                if (res.devices.length == 0) {
                  showToast('未搜索到蓝牙设备,请稍后再试')
                } else {
                  that.setData({
                    msg: "搜索设备" + JSON.stringify(res.devices),
                    devices: res.devices,
                    devicesFlag: true
                  })
                  wx.hideLoading()//关闭加载框
                }


              }
            })

          }, 1000)



          //获取所有已发现的设备 end

        }
      })
      //开始搜索设备 end------


      //监听蓝牙适配器状态
      wx.onBluetoothAdapterStateChange(function (res) {
        that.setData({
          sousuo: res.discovering ? "在搜索。" : "未搜索。",
          status: res.available ? "可用。" : "不可用。",
        })
      })
    },
    fail: function () {
      console.log("调用失败,请打开蓝牙")
      showModal('调用失败,请打开手机蓝牙')
      wx.hideLoading()
    }
  })
}
//搜索设备 end

//消息提示框
function showLoading(msg) {
  wx.showLoading({
    title: msg,
  })
}

//模态窗口
function showModal(content) {
  wx.showModal({
    title: '提示',
    content: content,
    success: function (res) {
      if (res.confirm) {
        console.log('用户点击确定')
        wx.hideLoading()
      } else if (res.cancel) {
        console.log('用户点击取消')
      }
    }
  })
}

/***消息提示框 */
function showToast(title) {
  wx.showToast({
    title: title,
    icon: 'none',
    duration: 2000
  })

}

//停止搜索设备
function stopBluetoothDevicesDiscovery() {

  var that = this;
  wx.stopBluetoothDevicesDiscovery({
    success: function (res) {
      console.log("停止搜索设备" + JSON.stringify(res.errMsg))

    }
  })

}

/****
 * 接收设备返回
 */
function onBLECharacteristicValueChange(that) {
  wx.onBLECharacteristicValueChange(function (res) {
    console.log("接收数据长度:" + res.value.byteLength)
    console.log("jieshoudao:" + ab2hex(res.value))
    receData = receData + ab2hex(res.value)

    that.setData({
      jieshoudao: receData,
    })

  })

}