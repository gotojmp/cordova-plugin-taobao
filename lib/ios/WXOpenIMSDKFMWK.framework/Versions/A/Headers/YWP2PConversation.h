//
//  YWP2PConversation.h
//  
//
//  Created by huanglei on 14/12/17.
//  Copyright (c) 2014年 taobao. All rights reserved.
//

#import "YWConversation.h"


@class YWPerson;
@class YWIMCore;
/**
 单聊会话
 */
@interface YWP2PConversation : YWConversation

/**
 *  获取某个单聊会话
 *  @param WXOPerson 会话对象
 *  @param aCreateIfNotExist 如果还不存在，则创建会话
 *  @return 需要获取的会话。如果本地不存在该会话且不需要新建，则返回nil
 */
+ (instancetype)fetchConversationByPerson:(YWPerson *)person creatIfNotExist:(BOOL)aCreateIfNotExist baseContext:(YWIMCore *)aBaseContext;

/**
 *  该单聊会话的person对象
 */
@property (nonatomic, strong, readonly) YWPerson *person;

@end
