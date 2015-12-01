//
//  TaeSDK+ALBBTrade.h
//  ALBBTradeSDK
//
//  Created by 友和(lai.zhoul@alibaba-inc.com) on 15/2/28.
//  Copyright (c) 2015年 Alipay. All rights reserved.
//

#import <Foundation/Foundation.h>


#import "ALBBItemService.h"
#import "ALBBOrderService.h"
#import "ALBBCartService.h"
#import "ALBBPromotionService.h"

#import "TaeH5PageServiceProtocol.h"



@interface TaeSDK(ALBBTrade)<ALBBItemService,ALBBOrderService,ALBBPromotionService,ALBBCartService,TaeH5PageServiceProtocol>



/**
 * 此方法不建议使用，请使用 showItemDetailByOpenId
 * 打开商品详情页面
 *
 *  @param parentController  app当前的Controller
 *  @param isNeedPush            是否需要使用parentController进行push
 *  @param webViewUISettings        可以自定义的webview配置项
 *  @param itemId                      商品混淆id
 *  @param itemType                      商品类型:1代表淘宝，2代表天猫
 *  @param params                        商品详情页请求附加参数
 *  @param tradeProcessSuccessCallback    交易流程成功完成订单支付的回调
 *  @param tradeProcessFailedCallback  交易流程未完成的回调
 */
-(void)showItemDetail:(UIViewController*)parentController
           isNeedPush:(BOOL) isNeedPush
    webViewUISettings:(TaeWebViewUISettings *)webViewUISettings
               itemId:(NSString *)itemId
             itemType:(NSInteger) itemType
               params:(NSDictionary *)params
tradeProcessSuccessCallback:(tradeProcessSuccessCallback)tradeProcessSuccessCallback
tradeProcessFailedCallback:(tradeProcessFailedCallback)tradeProcessFailedCallback;


/**
 *  此方法不建议使用，请使用 showItemDetailByOpenId
 *  以淘客方式打开商品详情页面
 *
 *  @param parentController  app当前的Controller
 *  @param isNeedPush            是否需要使用parentController进行push
 *  @param webViewUISettings        可以自定义的webview配置项
 *  @param itemId                      商品混淆id
 *  @param itemType                      商品类型:1代表淘宝，2代表天猫
 *  @param params                        商品详情页请求附加参数
 *  @param taoKeParams                        淘客参数
 *  @param tradeProcessSuccessCallback    交易流程成功完成订单支付的回调
 *  @param tradeProcessFailedCallback  交易流程未完成的回调
 */
-(void)showTaoKeItemDetail:(UIViewController*)parentController
                isNeedPush:(BOOL) isNeedPush
         webViewUISettings:(TaeWebViewUISettings *)webViewUISettings
                    itemId:(NSString *)itemId
                  itemType:(NSInteger) itemType
                    params:(NSDictionary *)params
               taoKeParams:(TaeTaokeParams *) taoKeParams
tradeProcessSuccessCallback:(tradeProcessSuccessCallback)tradeProcessSuccessCallback
tradeProcessFailedCallback:(tradeProcessFailedCallback)tradeProcessFailedCallback;




/**
 *  检查是否可以跳转到手机淘宝的detail,如果可以，直接跳转
 *
 *  @param itemId      商品真实id
 *  @param params      额外参数
 *  @param taoKeParams 淘客参数
 *
 *  @return <#return value description#>
 */
-(BOOL)  checkJumpTBDetail: (NSString *)itemId
                    params:(NSDictionary *)params
               taoKeParams:(TaeTaokeParams *) taoKeParams;


@end
