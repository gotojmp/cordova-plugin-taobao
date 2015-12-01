//
//  YWConversationServiceDef.h
//  
//
//  Created by huanglei on 14/12/18.
//  Copyright (c) 2014年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "YWServiceDef.h"
@class YWPerson;

/**
 *  收到新消息的回调
 *  @param aMessages 收到的新消息，数组中包含的对象都遵循 IYWMessage 协议，您可以直接访问 IYWMessage 中的方法
 */
typedef void(^YWConversationOnNewMessageBlock)(NSArray *aMessages);

/**
 *  收到新消息的回调
 *  @param aMessages 收到的新消息，数组中包含的对象都遵循 IYWMessage 协议，您可以直接访问 IYWMessage 中的方法
 *  @param aIsOffline 是否离线消息
 */
typedef void(^YWConversationOnNewMessageBlockV2)(NSArray *aMessages, BOOL aIsOffline);

typedef NS_ENUM(NSInteger, YWConversationInputStatus) {
    YWConversationStopInputStatus = 0x0,
    YWConversationInputTextStatus = 0x1,
    YWConversationRecordingStatus = 0x2,
    YWConversationSelectImageStatus = 0x4,
    YWConversationTakePictureStatus = YWConversationSelectImageStatus,
};

/**
 *  输入状态改变的回调
 *  @param aMessages 收到的新消息
 */
typedef void(^YWConversationInputStatusChangeBlock)(YWPerson *person, YWConversationInputStatus status);


/**
 *  数据集发生变更的回调，用于变更前或者变更完成时
 */
typedef void (^YWContentChangeBlock)(void);


typedef void (^YWResetContentBlock)(void);
/**
 *  数据集变更的回调，用于变更过程。
 *  @param object 被变更的对象
 *  @param indexPath 对象原先的位置
 *  @param newIndexPath 对象的新位置
 *  @param type 变更的方式，参见： YWObjectChangeType。
 */
typedef void (^YWObjectChangeBlock)(id object, NSIndexPath *indexPath, YWObjectChangeType type, NSIndexPath *newIndexPath);

@interface YWConversationServiceDef : NSObject

@end
