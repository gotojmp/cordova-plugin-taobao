//
//  TBOpenUtils.h
//  WopcMiniSDK
//
//  Created by muhuai on 15/8/19.
//  Copyright (c) 2015年 TaoBao. All rights reserved.
//

#import "TBBasicParam.h"
#import <UIKit/UIKit.h>
#import "TBAppLinkSDK.h"

/**
 *  SDK内部工具类
 */
@interface TBOpenUtils : NSObject


/**
 *  把dictionary转为json
 *
 */
+ (NSString *) pareseToJSONWithDictionary:(NSDictionary *)dictionary;

/**
 *  编码url
 */
+ (NSString *) encodeWithURL:(NSString *)url;

/**
 *  获取设备信息
 */
+ (NSDictionary *) getAppInfo;

/**
 *  获取当前时间 格式:YYMMddHHmmssSSSS 精确到毫秒
 */
+ (NSString *)getDateStamp;

/**
 *  query转Dictionary
 */
+ (NSDictionary*)queryDictionaryFromURL:(NSString*)urlString;

/**
 *  md5加密算法,传入待加密string
 */
+ (NSString *)encodeToMD5:(NSString *)string;
+ (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString;

@end
