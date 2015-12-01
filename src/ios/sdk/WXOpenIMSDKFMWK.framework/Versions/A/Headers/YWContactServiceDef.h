//
//  YWContactServiceDef.h
//  WXOpenIMSDK
//
//  Created by huanglei on 15/7/20.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@class YWPerson;


#pragma mark - Profile相关定义

/**
 *  获取到部分profile，比如先获取到了昵称，或者先获取到了头像。可以先调用这个block，告知IMSDK。
 *  @param aPerson Person对象
 *  @param aDisplayName 显示名称
 *  @param aAvatarImage 头像
 */
typedef void(^YWFetchProfileProgressBlock)(YWPerson *aPerson, NSString *aDisplayName, UIImage *aAvatarImage);

/**
 *  完成获取Profile后，通过这个回调，通知IMSDK
 *  @param aIsSuccess 是否成功
 *  @param aPerson Person对象
 *  @param aDisplayName 显示名称
 *  @param aAvatarImage 头像
 */
typedef void(^YWFetchProfileCompletionBlock)(BOOL aIsSuccess, YWPerson *aPerson, NSString *aDisplayName, UIImage *aAvatarImage);


#pragma mark - 联系人列表相关定义

/**
 *  联系人列表划分方式
 */
typedef enum {
    /// 按字母划分
    YWContactListModeAlphabetic,
    /// 按分组划分
    YWContactListModeGroup,
} YWContactListMode;
