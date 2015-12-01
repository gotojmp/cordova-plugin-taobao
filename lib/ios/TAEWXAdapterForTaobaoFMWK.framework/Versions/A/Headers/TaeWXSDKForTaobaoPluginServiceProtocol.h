//
//  TaeWXSDKForTaobaoPluginServiceProtocol.h
//  TaeWXSDKForTaobaoPluginAdapter
//
//  Created by 友和(lai.zhoul@alibaba-inc.com) on 14/11/26.
//  Copyright (c) 2014年 Alipay. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <UIKit/UIKit.h>
#import <WXOpenIMSDKFMWK/YWFMWK.h>


@protocol TaeWXSDKForTaobaoPluginServiceProtocol <NSObject>

/**
 *  打开旺旺会话列表
 */
- (void)openConversationListFromController:(UIViewController *)aFromController;

/**
 * 打开某个旺旺会话
 */
- (void)openConversationView:(NSString *)aConversationId fromController:(UIViewController *)aFromController;

/**
 *  获取当前登录旺旺账号的总未读数
 *  如果未登录，则返回0
 */
- (NSNumber *)totalUnreadCount;

/**
 *  获取当前kit
 *  如果未登录，则返回nil
 */
- (YWIMKit *)currentKit;

/**
 *  登录
 */
- (void)asyncLoginWithCompletionBlock:(YWCompletionBlock)aCompletionBlock;

/**
 *  新消息提醒的回调，如果你没有设置该回调，IMSDK将显示默认的提示样式
 *  如果当前正停留在新消息所在会话，则不会进入该回调
 *  @param aTips 提示文案
 *  @param aConversationIdsArray 新消息所属会话Id的数组。（可能不止一条新消息，并且来自于不同会话）
 */
typedef void(^WXAdapterOnNewMessagesBlock)(NSString *aTips, NSArray *aConversationIdsArray);


@property (nonatomic, copy, readonly) WXAdapterOnNewMessagesBlock onNewMessagesBlock;
/**
 *  设置新消息提醒的回调
 */
- (void)setOnNewMessagesBlock:(WXAdapterOnNewMessagesBlock)onNewMessagesBlock;





#pragma mark - 多帐号模式相关

/**
 *  如果您的app启用多帐号模式，即既需要跟卖家的旺旺帐号聊天，又需要使用OpenIM的帐号体系聊天
 *  那么请务必在-[AppDelegate application:didFinishLaunchingWithOptions:]中发送此通知，告知IMSDK
 */
#define WXSDKMultiAccountModeNotification @"WXSDKMultiAccountModeNotification"

@property (nonatomic, copy, readonly) YWPushHandleResultBlockV3 onClickPushNotificationForFreeBlock;
/**
 *  多帐号模式下，用户点击Push消息的回调
 */
- (void)setOnClickPushNotificationForFreeBlock:(YWPushHandleResultBlockV3)onClickPushNotificationForFreeBlock;



@end
