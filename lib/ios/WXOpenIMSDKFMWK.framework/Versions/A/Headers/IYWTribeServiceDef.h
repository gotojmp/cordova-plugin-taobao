//
//  IYWTribeServiceDef.h
//  WXOpenIMSDK
//
//  Created by huanglei on 15/9/15.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "YWTribe.h"

@interface IYWTribeServiceDef : NSObject

/**
 *  群聊管理接口较多且接口参数复杂，这里汇总定义了不同接口中的UserInfoKey定义
 *  具体含义请参看 各个接口 自身的定义
 */
FOUNDATION_EXTERN NSString *const YWTribeServiceKeyTribeId;

FOUNDATION_EXTERN NSString *const YWTribeServiceKeyTribeNotice;

FOUNDATION_EXTERN NSString *const YWTribeServiceKeyTribeName;

FOUNDATION_EXTERN NSString *const YWTribeServiceKeyTipInfo;

/// 对应的Value为YWPerson对象
FOUNDATION_EXTERN NSString *const YWTribeServiceKeyPerson;

/// 对应的Value为YWPerson对象
FOUNDATION_EXTERN NSString *const YWTribeServiceKeyPerson2;

/// 系统的消息的处理状态, Value 为 YWMessageBodyTribeSystemStatus 类型转化成的 NSNumber
FOUNDATION_EXTERN NSString *const YWTribeServiceKeyStatus;

FOUNDATION_EXTERN NSString *const YWTribeServiceKeyTribeType;

@end


/**
 *  这个结构体用于创建群聊等接口
 */
@interface YWTribeDescriptionParam : NSObject

/// 详见类型注释
@property (nonatomic, assign) YWTribeType tribeType;

@property (nonatomic, copy) NSString *tribeName;

@property (nonatomic, copy) NSString *tribeNotice;

/// 必须是包含YWPerson对象的数组
@property (nonatomic, strong) NSArray *tribeMembers;

/// 校验模式，在普通类型的群有效，详见该类型注释
@property (nonatomic, assign) YWTribeCheckMode tribeCheckMode;

@end