//
//  IYWPushService.h
//  
//
//  Created by huanglei on 14/12/16.
//  Copyright (c) 2014年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol IYWMessage;
@class YWPerson;

/// Push服务错误域
FOUNDATION_EXTERN NSString *const YWPushServiceDomain;

/**
 Push服务
 */

@protocol IYWPushService <NSObject>

/**
 *  获取转换后的deviceToken字符串
 */
@property (nonatomic, readonly) NSString *deviceTokenString;

#pragma mark - V3

/**
 *  Push处理回调
 *  @param aIsLaunching 是否是用户划开push启动app（app原先处于未启动状态）
 *  @param aState app当前状态，您可以判断是否是由于用户划开push，导致app从后台进入前台
 *  @param aAPS APS消息本身
 *  @param aConversationId 会话Id
 *  @param aConversationClass 会话类型，返回值可能是：[YWP2PConversation class]、[YWTribeConversation class]
 *  @param aToPerson 供调试使用，你一般不需要关心这个参数；消息是发送给哪个帐号
 */
typedef void(^YWPushHandleResultBlockV3)(BOOL aIsLaunching, UIApplicationState aState, NSDictionary *aAPS, NSString *aConversationId, Class aConversationClass, YWPerson *aToPerson);

@property (nonatomic, copy, readonly) YWPushHandleResultBlockV3 handlePushBlockV3;

/**
 *  现在IMSDK可以自己监听到Push事件，您只需要在-[AppDelegate application:didFinishLaunchingWithOptions:]中设置下面这个回调，做对应的处理。
 *  @note 您不需要再调用V2或者V1版本中的handleXXX函数，在 application:didFinishLaunchingWithOptions 中实现 setHandlePushBlockV3: 回调即可
 */
- (void)setHandlePushBlockV3:(YWPushHandleResultBlockV3)handlePushBlockV3;


#pragma mark - deprecated

/**
 *  现在当你的app获取到DeviceToken时，IMSDK会截获到该DeviceToken，自动使用该DeviceToken，您不需要调用此接口设置。
 */
@property (nonatomic, copy, readwrite) NSData *deviceToken;


#pragma mark - V1

/**
 *  Push处理回调
 *  @param aAPS APS消息本身
 *  @param aConversationId 会话Id
 */
typedef void(^YWPushHandleResultBlock)(NSDictionary *aAPS, NSString *aConversationId);

/**
 *  处理启动参数
 *  @param aLaunchOptions 启动参数
 *  @param aCompletionBlock 如果OpenIM接受处理该启动参数，在处理完毕后通过此回调返回结果
 *  @return OpenIM是否接受处理该启动参数
 */
- (BOOL)handleLaunchOptions:(NSDictionary *)aLaunchOptions completionBlock:(YWPushHandleResultBlock)aCompletionBlock __attribute__((deprecated("您不需要再调用V2或者V1版本中的handleXXX函数，在 application:didFinishLaunchingWithOptions 中实现 setHandlePushBlockV3: 回调即可")));

/**
 *  处理Push消息
 *  @param aPushUserInfo
 *  @param aCompletionBlock
 *  @return OpenIM是否接受处理该Push
 */
- (BOOL)handlePushUserInfo:(NSDictionary *)aPushUserInfo completionBlock:(YWPushHandleResultBlock)aCompletionBlock __attribute__((deprecated("您不需要再调用V2或者V1版本中的handleXXX函数，在 application:didFinishLaunchingWithOptions 中实现 setHandlePushBlockV3: 回调即可")));


#pragma mark - V2

/**
 *  Push处理回调
 *  @param aAPS APS消息本身
 *  @param aConversationId 会话Id
 *  @param aConversationClass 会话类型，返回值可能是：[YWP2PConversation class]、[YWTribeConversation class]
 */
typedef void(^YWPushHandleResultBlockV2)(NSDictionary *aAPS, NSString *aConversationId, Class aConversationClass);

/**
 *  处理启动参数
 *  @param aLaunchOptions 启动参数
 *  @param aCompletionBlock 如果OpenIM接受处理该启动参数，在处理完毕后通过此回调返回结果
 *  @return OpenIM是否接受处理该启动参数
 */
- (BOOL)handleLaunchOptionsV2:(NSDictionary *)aLaunchOptions completionBlock:(YWPushHandleResultBlockV2)aCompletionBlock __attribute__((deprecated("您不需要再调用V2或者V1版本中的handleXXX函数，在 application:didFinishLaunchingWithOptions 中实现 setHandlePushBlockV3: 回调即可")));

/**
 *  处理Push消息
 *  @param aPushUserInfo
 *  @param aCompletionBlock
 *  @return OpenIM是否接受处理该Push
 */
- (BOOL)handlePushUserInfoV2:(NSDictionary *)aPushUserInfo completionBlock:(YWPushHandleResultBlockV2)aCompletionBlock __attribute__((deprecated("您不需要再调用V2或者V1版本中的handleXXX函数，在 application:didFinishLaunchingWithOptions 中实现 setHandlePushBlockV3: 回调即可")));


@end
