//
//  YWImageBrowserHelper.h
//  WXOpenIMUIKit
//
//  Created by huanglei on 15/9/7.
//  Copyright (c) 2015年 www.alibaba.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol IYWMessage;
@class YWConversation;
@class YWIMKit;

FOUNDATION_EXTERN NSString *const YWImageBrowserHelperActionKeyMessageId;
FOUNDATION_EXTERN NSString *const YWImageBrowserHelperActionKeyConversationId;

@interface YWImageBrowserHelper : NSObject

/**
 *  这个函数用来预览图片消息的大图
 *  @param aMessage 用户点击的图片消息
 *  @param aConversation 所属会话
 *  @param aNavigationController 用于Push的导航栏控制器
 *  @param aNeedDefaultSaveAction 是否需要默认的保存按钮
 *  @param aAdditionalActions 更多选项，数组中每一个对象都必须是YWMoreActionItem对象，用户点击回调的UserInfo中使用 YWImageBrowserHelperActionKeyMessage 作为Key，传递当前操作的id<IYWMessage>对象
 */
+ (void)previewImageMessage:(id<IYWMessage>)aMessage
               conversation:(YWConversation *)aConversation
     inNavigationController:(UINavigationController *)aNavigationController
          additionalActions:(NSArray *)aAdditionalActions;


/**
 *  这个函数用来预览图片消息的大图
 *  @param aMessage 用户点击的图片消息
 *  @param aConversation 所属会话
 *  @param aNavigationController 用于Push的导航栏控制器
 *  @param aNeedDefaultSaveAction 是否需要默认的保存按钮
 *  @param aAdditionalActions 更多选项，数组中每一个对象都必须是YWMoreActionItem对象，用户点击回调的UserInfo中使用 YWImageBrowserHelperActionKeyMessage 作为Key，传递当前操作的id<IYWMessage>对象
 *  @param aIMKit 用于多账号同时登录的情况
 */
+ (void)previewImageMessage:(id<IYWMessage>)aMessage
               conversation:(YWConversation *)aConversation
     inNavigationController:(UINavigationController *)aNavigationController
          additionalActions:(NSArray *)aAdditionalActions
                  withIMKit:(YWIMKit *)aIMKit;


@end
