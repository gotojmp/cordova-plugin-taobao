//
//  IYWContactService.h
//  WXOpenIMSDK
//
//  Created by huanglei on 15/7/20.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "YWContactServiceDef.h"
#import "YWServiceDef.h"

@class YWPerson;


/**
 *  联系人是指建立关系的人
 *  联系人服务提供了联系人列表、联系人管理等功能
*/
@protocol IYWContactService <NSObject>

/**
 *  在线状态
 */

/// 是否启用在线状态功能
/// 默认为NO，即所有用户均显示为在线
@property (nonatomic, assign) BOOL enableContactOnlineStatus;

/// 联系人在线状态变化的回调
/// 当这个block返回NO时，意味着不需要再响应，从此以后将不会再调用这个block
typedef BOOL (^YWOnlineStatusChangeBlock)(YWPerson *person, BOOL onlineStatus);
typedef void (^YWOnlineStatusDidUpdateBlock)(BOOL isSuccess);

/// 获取缓存中的在线状态，不会发起网络请求,
- (NSDictionary *)getOnlineStatusForPersons:(NSArray *)persons;
/// 获取缓存中的在线状态，如果有缓存过期，则会发起网络请求，请求返回后回调block，且只回调一次
//  @param persons 需要获取在线状态的person数组
//  @param expire  过期时间，距离上次更新时间超过此参数的缓存将被认为是过期，从而触发网络请求
//  @param block   回调
- (void)updateOnlineStatusForPersons:(NSArray *)persons withExpire:(NSTimeInterval)expire andOnlineStatusUpdateBlock:(YWOnlineStatusDidUpdateBlock)block;

/// 添加对某个person在线状态改变的监听
- (void)addPersonOnlineStatusChanged:(NSArray *)persons withBlock:(YWOnlineStatusChangeBlock)block forKey:(NSString *)aKey ofPriority:(YWBlockPriority)aPriority;
/// 移除对某个person在线状态改变的监听
- (void)removePersonOnlineStatusChanged:(YWPerson *)person forKey:(NSString *)aKey;
/// 移除某个key对应的所有block
- (void)removePersonOnlineStatusChangedBlockForKey:(NSString *)aKey;


/**
 *  黑名单
 */

/// 黑名单操作的回调，当error == nil或者error.errorCode为0时表示成功。
typedef void (^YWBlackListResultBlock) (NSError *error, YWPerson *person);
typedef void (^YWFetchBlackListCompletionBlock) (NSError *error, NSArray *blackList);
/// 添加某个person到黑名单
- (void)addPersonToBlackList:(YWPerson *)person withResultBlock:(YWBlackListResultBlock)resultBlock;
/// 将某个person从黑名单中移除
- (void)removePersonFromBlackList:(YWPerson *)person withResultBlock:(YWBlackListResultBlock)resultBlock;
/// 获取一定数量黑名单
- (void)fetchBlackListWithCompletionBlock:(YWFetchBlackListCompletionBlock)completionBlock;
/// 判断某个person是否在黑名单中
- (BOOL)isPersonInBlackList:(YWPerson *)person;


/**
 *  服务端Profile
 *  @note 如果你导入账号时，同时导入了用户Profile，则可以通过此接口获取你导入的用户profile
 */
- (void)asyncGetProfileForPerson:(YWPerson *)person
                        progress:(YWFetchProfileProgressBlock)progressBlock
                 completionBlock:(YWFetchProfileCompletionBlock)completionBlock;



/**
 *  联系人列表，目前只支持淘宝域。第三方OpenIM并没有此服务，OpenIM开发者自己维护好友关系。
 */

/**
 *  获取不同排序和分组模式的FRC对象
 */
- (YWFetchedResultsController *)fetchedResultsControllerWithListMode:(YWContactListMode)aMode imCore:(YWIMCore *)imCore;
/**
 *  用于搜索用户id
 */
- (YWFetchedResultsController *)fetchedResultsControllerWithSearchKeyword:(NSString *)aKeyword imCore:(YWIMCore *)imCore;



@end
