//
//  YWCustomMsgModule.h
//  WXOpenIMSDK
//
//  Created by sidian on 15/6/29.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import "YWConversation.h"

/// 开发者自定义会话id的前缀必须是这个，否则无法创建成功
#define kYWCustomConversationIdPrefix   @"ywcustom"

@interface YWCustomConversation : YWConversation


/**
 *  获取或者创建某个自定义会话
 *  @param aConversationId 会话Id
 *  @param aCreateIfNotExist 如果还不存在，则创建会话
 *  @return 需要获取的会话。如果本地不存在该会话且不需要新建，则返回nil
 *  @note   开发者自定义会话id的前缀必须是kYWCustomConversationIdPrefix，否则无法创建成功
 */
+ (instancetype)fetchConversationByConversationId:(NSString *)aConversationId creatIfNotExist:(BOOL)aCreateIfNotExist baseContext:(id)aBaseContext;


/**
 *  修改该自定义会话的未读数，最后条消息内容，最后条消息时间
 *  @note 由于需要保证数据一致性，所以该操作会放到异步队列
 *  @note 不需要修改的项，直接传nil
 */
- (void)modifyUnreadCount:(NSNumber *)aUnreadCount latestContent:(NSString *)aLatestContent latestTime:(NSDate *)aLatestTime;


@end
