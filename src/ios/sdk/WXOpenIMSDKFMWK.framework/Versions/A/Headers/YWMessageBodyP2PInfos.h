//
//  YWMessageBodyP2PInfos.h
//  WXOpenIMSDK
//
//  Created by sidian on 15/10/8.
//  Copyright © 2015年 taobao. All rights reserved.
//

#import "YWMessageBody.h"

/// 宝贝焦点消息
@interface YWMessageBodyP2PInfos : YWMessageBody

/// 初始化
- (instancetype)initWithItemId:(NSString *)itemId;

/// 宝贝ID
@property (nonatomic, copy, readonly) NSString *itemId;

@end
