//
//  YWMessageBodyTribeSystem.h
//  
//
//  Created by Jai Chen on 15/1/26.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import "YWMessageBody.h"


/**
 *  群系统消息的子类型
 */
typedef NS_ENUM(NSUInteger, YWMessageBodyTribeSystemType){
    /**
     *  普通文本的系统消息
     *  通过参数 userInfo 返回的普通文本信息，userInfo 中包含的 Key 为：YWTribeServiceKeyTipInfo（主体为NSString文本）
     */
    YWMessageBodyTribeSystemTipInfo                 = 0x5,// 系统消息
    /**
     *  成员被邀请的消息
     *  通过参数 userInfo 返回更新信息，userInfo 中包含的 Key 为: YWTribeServiceKeyPerson (主体YWPerson对象)
     */
    YWMessageBodyTribeSystemTypeInvited = 0x20,
    /**
     *  成员加入消息
     *  通过参数 userInfo 返回更新信息，userInfo 中包含的 Key 为: YWTribeServiceKeyPerson (主体YWPerson对象)
     */
    YWMessageBodyTribeSystemTypeJoined = 0x21,
    /**
     *  成员退出消息
     *  通过参数 userInfo 返回更新信息，userInfo 中包含的 Key 为: YWTribeServiceKeyPerson (主体YWPerson对象)
     */
    YWMessageBodyTribeSystemTypeExited = 0x22,
    /**
     *  成员被踢出消息
     *  通过参数 userInfo 返回更新信息，userInfo 中包含的 Key 为: YWTribeServiceKeyPerson (主体YWPerson对象)
     */
    YWMessageBodyTribeSystemTypeExpelled = 0x23,

    /**
     *  群名称或群公告更新消息
     *  通过参数 userInfo 返回更新信息，userInfo 中可能包含的 Key 为: YWTribeServiceKeyTribeId (群Id)、YWTribeServiceKeyTribeName (群名称)、YWTribeServiceKeyTribeNotice (群公告)，当某信息被更新时，userInfo 将包含相应的键值对
     */
    YWMessageBodyTribeSystemTypeTribeInfoUpdated = 0x26,
    
    /**
     *  群主转让消息
     *  通过参数 userInfo 返回更新信息，userInfo 中包含的 Key 为: YWTribeServiceKeyPerson (主体YWPerson对象)
     */
    YWMessageBodyTribeSystemTypeAddManager       = 0x120, /// 群主转让
    /**
     *  取消管理员消息
     *  通过参数 userInfo 返回更新信息，userInfo 中包含的 Key 为: YWTribeServiceKeyPerson (主体YWPerson对象)
     */
    YWMessageBodyTribeSystemTypeDelManager       = 0x121, /// 取消管理员


};

typedef NS_ENUM(NSUInteger, YWMessageBodyTribeSystemStatus) {
    /**
     *  缺省，不需要处理
     */
    YWMessageBodyTribeSystemStatusNone                   = 0x0,
    /**
     *  已加入
     */
    YWMessageBodyTribeSystemStatusAdded                  = 0x1,
    /**
     *  已忽略
     */
    YWMessageBodyTribeSystemStatusIgnored                = 0x2,
    /**
     *  待处理
     */
    YWMessageBodyTribeSystemStatusWait2BProcess          = 0x3,
};

/**
 * 群系统消息体
 */
@interface YWMessageBodyTribeSystem : YWMessageBody

/**
 *  群系统消息的子类型
 */
@property (nonatomic, readonly) YWMessageBodyTribeSystemType tribeSystemType;

/**
 *  提供具体信息的字典
 */
@property (nonatomic, readonly) NSDictionary *userInfo;

@end

/**
 *  WXOMessageBodyTribeSystem 中 userInfo 字典的键，用于获取具体信息
 *  这些键值已经过时，请使用 IYWTribeServiceDef.h 中的定义
 */
#define kYWMessageBodyTribeSystemUserInfoKeyPerson      @"person"
#define kYWMessageBodyTribeSystemUserInfoKeyTribeName   @"tribeName"
#define kYWMessageBodyTribeSystemUserInfoKeyNotice      @"notice"
