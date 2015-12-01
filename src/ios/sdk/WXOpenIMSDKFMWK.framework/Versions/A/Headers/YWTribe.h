//
//  YWTribe.h
//
//
//  Created by Jai Chen on 15/1/13.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 群校验模式（仅限普通群，暂时不校验，后续扩展）
 */
typedef NS_ENUM(NSUInteger, YWTribeCheckMode) {
    
    /// 不校验
    YWTribeCheckModeNone = 0
};
/**
 群类型
 */
typedef NS_ENUM(NSInteger, YWTribeType)
{
    YWTribeTypeNormal       = 0,   // 普通群（a.邀请人加入时需对方确认，b.支持加入群的校验模式）
    YWTribeTypeMultipleChat = 1,   // 多聊群
};
/**
 *  群对象
 */
@interface YWTribe : NSObject <NSCoding>

/**
 *  群的唯一标识符
 */
@property (readonly, copy, nonatomic) NSString *tribeId;

/**
 *  群名称
 */
@property (readonly, copy, nonatomic) NSString *tribeName;

/**
 *  群公告
 */
@property (readonly, copy, nonatomic) NSString *notice;

/**
 *  群类型
 */
@property (readonly, assign, nonatomic) YWTribeType tribeType;


- (BOOL)isEqualToTribe:(YWTribe *)tribe;

@end
