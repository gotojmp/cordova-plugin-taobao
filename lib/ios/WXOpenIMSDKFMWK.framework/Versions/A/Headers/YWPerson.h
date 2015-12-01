//
//  YWPerson.h
//  
//
//  Created by huanglei on 14/12/28.
//  Copyright (c) 2014年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>

@class YWIMCore;

@interface YWPerson : NSObject
<NSCoding, NSCopying>

/**
 *  初始化，生成默认的App内聊天对象
 *  @param aPersonId personId
 *  @return WXOPerson对象
 */
- (instancetype)initWithPersonId:(NSString *)aPersonId;

/**
 *  初始化，可以指定聊天对象所属App
 *  @param aPersonId 对象id
 *  @param aAppKey 聊天对象所属App
 */
- (instancetype)initWithPersonId:(NSString *)aPersonId appKey:(NSString *)aAppKey;


/**
 *  该聊天对象的personId
 */
@property (nonatomic, copy, readonly) NSString *personId;

/**
 *  该聊天对象所属appkey
 */
@property (nonatomic, copy, readonly) NSString *appKey;

/**
 *  检查与 aPerson 是否表示同一对象
 */
- (BOOL)isEqualToPerson:(YWPerson *)aPerson;

@end

@interface YWPerson ()

/**
 *  初始化，生成默认的App内聊天对象
 *  @note 本接口用于在不同SDK类型的账号同时使用时，指定IMCore
 *  @param aPersonId personId
 *  @return WXOPerson对象
 */
- (instancetype)initWithPersonId:(NSString *)aPersonId inBaseContext:(YWIMCore *)aIMCore;

@end

/**
 *  客服功能接口
 */
@interface YWPerson (EService)

/**
 *  创建客服聊天对象
 *  @brief 当您收到客服消息时，您可以判断该Person的appKey是否为YWSDKAppKey，从而对客服的头像等进行特定的显示。
 *  @param aPersonId 客服Id
 *  @param aGroupId 客服分组Id，如果没有对客服进行分组，则可以传入nil
 *  @return 客服的Person对象，您可以通过该Person对象创建会话并发起聊天
 */
- (instancetype)initWithPersonId:(NSString *)aPersonId EServiceGroupId:(NSString *)aGroupId baseContext:(YWIMCore *)aBaseContext;

/**
 *  是否子账号
 */
- (BOOL)isSubAccount;


@end



@interface YWPerson (PersonLongId)

//在多应用中唯一的标识一个用户，如果只有一个基于OpenIM的应用，开发者不需要关心
@property (nonatomic, copy, readonly) NSString *personLongId;

@end
