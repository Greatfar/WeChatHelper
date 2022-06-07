#pragma once


#define WxReciveMessage 0x4E9464        // 接收消息call 汇编call指令所在地址 偏移 （HOOK挂钩点）
#define WxReciveMessageCall 0xC9C90     // 被HOOK覆盖的汇编call指令 所指向的call（函数）地址的偏移
// 聊天消息 偏移
#define MsgTypeOffset 0x30                // 消息类型的偏移 
#define WxidOffset 0x48                    // 微信ID/群ID偏移 
#define MsgContentOffset 0x70            // 消息内容的偏移 
#define MsgSourceOffset 0x1AC            // 消息来源的偏移 
#define GroupMsgSenderOffset 0x170        // 群消息发送者偏移


#define LoginSign_Offset 0x222EFF8      // 微信登陆状态标识
#define WxLogout 0x60F840               // 退出微信call


// 个人信息 基址 
#define WxSelfInfoBase 0x222EBB4
// 昵称 ASCII字符
#define WxNickName WxSelfInfoBase
// 手机号 ASCII字符
#define WxPhoneNumber WxSelfInfoBase+0x34
// 国家 ASCII字符
#define WxNation WxSelfInfoBase+0x1F4
// 省 ASCII字符
#define WxProvince WxSelfInfoBase+0x108
// 市  ASCII字符
#define WxCity WxSelfInfoBase+0x120
// 微信号 ASCII字符
#define WxCount WxSelfInfoBase+0x454
// 登陆设备 ASCII字符
#define WxDevice WxSelfInfoBase+0x4A4
// 微信ID ASCII指针
#define WxID WxSelfInfoBase+0x46C
// 大头像 ASCII指针
#define WxBigHeader WxSelfInfoBase+0x2E0
// 小头像 ASCII指针
#define WxSmallHeader WxSelfInfoBase+0x2F8

