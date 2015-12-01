//
//  TBBasicParam.h
//  WopcMiniSDK
//
//  Created by muhuai on 15/8/18.
//  Copyright (c) 2015年 TaoBao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TBError.h"

@interface TBBasicParam : NSObject

/**
 *  返回的跳转地址(可选)
 */
@property (nonatomic, strong) NSString              *backURL;

/**
 *  e,type 淘客App使用参数,非淘客App请忽略
 */
@property (nonatomic, strong) NSString              *e;
@property (nonatomic, strong) NSString              *type;
/**
 * 签名，将appLink作为安全通道使用时传入的参数，加签算法支持：黑匣子；如有其他安全算法，可提供server端解密算法联系@酒仙
 */
@property (nonatomic, strong) NSString              *sign;

/**
 *  毫秒时间戳
 */
@property (nonatomic, strong) NSString              *time;

/**
 *  额外参数(可选),类型NSDictionary,使用addEntriesFromDictionary 来追加参数,不要直接覆盖
 */
@property (nonatomic, strong) NSMutableDictionary *extraParam;


/**
 *  把所需params转换成string
 *
 *  @return
 */
- (NSString *)parseToString;

/**
 *  降级H5时调用该方法,父类里没有实现,具体化的子类里会复写这个方法
 */
- (NSString *)parseToH5URL;

- (TBError *)isLegalParam;

@end
