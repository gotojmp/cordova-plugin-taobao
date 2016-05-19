/*
 Licensed to the Apache Software Foundation (ASF) under one
 or more contributor license agreements.  See the NOTICE file
 distributed with this work for additional information
 regarding copyright ownership.  The ASF licenses this file
 to you under the Apache License, Version 2.0 (the
 "License"); you may not use this file except in compliance
 with the License.  You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing,
 software distributed under the License is distributed on an
 "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND, either express or implied.  See the License for the
 specific language governing permissions and limitations
 under the License.
 */

#import "CDVTaobao.h"

#pragma mark CDVTaobao

@implementation CDVTaobao

- (void)pluginInitialize
{
    [[ALBBSDK sharedInstance] setDebugLogOpen:NO];
    [[ALBBSDK sharedInstance] setUseTaobaoNativeDetail:NO];
    //[CloudPushSDK setEnvironment:CCPSDKEnvironmentRelease];
    
    [[ALBBSDK sharedInstance] asyncInit:^{
        NSLog(@"初始化成功");
    } failure:^(NSError *error) {
        NSLog(@"初始化失败:%@", error);
    }];
    
    [self initServices];
    
    [self registerAPNS];
    [self registerMsgReceive];
    
    [self setSSOAppKey];

    //[self initSSOLoginHandler];
    
    [self initTaobaoTrade];
}

- (void)initServices
{
    _loginService = [[ALBBSDK sharedInstance]getService:@protocol(ALBBLoginService)];
    _itemService = [[ALBBSDK sharedInstance]getService:@protocol(ALBBItemService)];
    _cartService = [[ALBBSDK sharedInstance]getService:@protocol(ALBBCartService)];
    _tradeService = [[ALBBSDK sharedInstance]getService:@protocol(ALBBTradeService)];
    _mediaService = [[ALBBSDK sharedInstance] getService:@protocol(ALBBMediaServiceProtocol)];
    _ssoService = [[ALBBSDK sharedInstance]getService:@protocol(ALBBOpenAccountSSOServiceProtocol)];
}

- (void)initSSOLoginHandler
{
    //设置云账号的登入和登出状态监听器
    [[ALBBOpenAccountSession sharedInstance] addOpenAccountStateChangeHandler:^(ALBBOpenAccountSession *currentSession) {
        if ([ALBBOpenAccountSession sharedInstance].isLogin) {
            //NSLog(@"token:%@,user:%@",[[ALBBOpenAccountSession sharedInstance] getAuthToken],[[ALBBOpenAccountSession sharedInstance] getUser]);
        } else {
            //NSLog(@"token:%@,user:%@",[[ALBBOpenAccountSession sharedInstance] getAuthToken],[[ALBBOpenAccountSession sharedInstance] getUser]);
        }
    }];
}

- (void)initTaobaoTrade
{
    _taoKeParams = [[ALBBTradeTaokeParams alloc] init];
    _taoKeParams.pid = [[self.commandDelegate settings] objectForKey:@"taokepid"];
    
    _uiSettings = [[TaeWebViewUISettings alloc]init];
    _uiSettings.titleColor = [UIColor redColor];
    _uiSettings.tintColor = [UIColor redColor];
    _uiSettings.barTintColor = [UIColor whiteColor];
    
    __weak __typeof(&*self)me = self;
    _tradeProcessSuccessCallback=^(ALBBTradeResult *tradeProcessResult){
        NSLog(@"交易成功，成功的订单：%@，失败的订单：%@",tradeProcessResult.paySuccessOrders,tradeProcessResult.payFailedOrders);
        NSMutableArray* so = [[NSMutableArray alloc] init];
        NSMutableArray* fo = [[NSMutableArray alloc] init];
        for (id orderId in tradeProcessResult.paySuccessOrders) {
            [so addObject:[NSString stringWithFormat:@"%@",orderId]];
        }
        for (id orderId in tradeProcessResult.payFailedOrders) {
            [fo addObject:[NSString stringWithFormat:@"%@",orderId]];
        }
        NSDictionary* result = @{
                             @"successOrders": so,
                             @"failedOrders": fo,
                             };
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:result];
        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId];
    };
    _tradeProcessFailedCallback=^(NSError *error){
        NSLog(@"交易失败，错误码是：%ld，错误原因是：%@",error.code,error.description);
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:error.description];
        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId];
    };
}

//上传多个图片会有问题，覆盖callbackid
- (void)uploadImage:(CDVInvokedUrlCommand*)command
{
    NSString* currentCallbackId = command.callbackId;
    __weak __typeof(&*self)me = self;
    
    NSString* mediaNameSpace = [[self.commandDelegate settings] objectForKey:@"taobao_media_ns"];
    
    NSString* dir = [command.arguments objectAtIndex:1];
    NSString* fileName = [command.arguments objectAtIndex:2];
    int imgType = [[command.arguments objectAtIndex:3] intValue];
    TFEUploadParameters* params;
    
    if (imgType) {
        NSString* imageId = [command.arguments objectAtIndex:0];
        NSString* imageAssetURL = [NSString stringWithFormat:@"assets-library://asset/asset.JPG?id=%@&ext=JPG",imageId];
        NSURL* fileURL = [NSURL URLWithString:imageAssetURL];
        params = [TFEUploadParameters paramsWithAssertUrl:fileURL
                                                    space:mediaNameSpace
                                                 fileName:fileName
                                                      dir:dir];
    } else {
        NSString* filePath = [command.arguments objectAtIndex:0];
        params = [TFEUploadParameters paramsWithFilePath:filePath
                                                   space:mediaNameSpace
                                                fileName:fileName
                                                     dir:dir];
    }
    
    [_mediaService upload:params notification:[TFEUploadNotification
                                               notificationWithProgress:^(TFEUploadSession *session, NSUInteger progress) {
                                                   NSLog(@"%@ - %lu", session.uniqueIdentifier, (unsigned long) progress);
                                               }
                                               success:^(TFEUploadSession *session, NSString *url) {
                                                   NSDictionary* image = @{
                                                                        @"url": url
                                                                        };
                                                   CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:image];
                                                   [me.commandDelegate sendPluginResult:pluginResult callbackId:currentCallbackId];
                                               }
                                               failed:^(TFEUploadSession *session, NSError *error) {
                                                   CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
                                                   [me.commandDelegate sendPluginResult:pluginResult callbackId:currentCallbackId];
                                               }
                                               ]];
}

-(void)setSSOAppKey
{
    NSString* uMengAppKey = [[self.commandDelegate settings] objectForKey:@"umeng_appkey_ios"];
    [UMSocialData setAppKey:uMengAppKey];
    [UMSocialData openLog:NO];
    //[UMSocialConfig hiddenNotInstallPlatforms:nil];
    
    NSString* wxAppId = [[self.commandDelegate settings] objectForKey:@"wx_appid"];
    NSString* wxAppSecret = [[self.commandDelegate settings] objectForKey:@"wx_appsecret"];
    NSString* wxAppUrl = [[self.commandDelegate settings] objectForKey:@"wx_appurl"];
    NSString* qqAppId = [[self.commandDelegate settings] objectForKey:@"qq_appid"];
    NSString* qqAppKey = [[self.commandDelegate settings] objectForKey:@"qq_appkey"];
    NSString* qqAppUrl = [[self.commandDelegate settings] objectForKey:@"qq_appurl"];
    NSString* wbAppKey = [[self.commandDelegate settings] objectForKey:@"wb_appkey"];
    NSString* wbCallbackUrl = [[self.commandDelegate settings] objectForKey:@"wb_callbackurl"];
    [_ssoService setWXAppId:wxAppId appSecret:wxAppSecret url:wxAppUrl];
    [_ssoService setQQWithAppId:qqAppId appKey:qqAppKey url:qqAppUrl];
    [_ssoService setWBAppKey:wbAppKey url:wbCallbackUrl];
}


- (void)shareTo:(CDVInvokedUrlCommand*)command
{
    self.currentCallbackId_share = command.callbackId;
    __weak __typeof(&*self)me = self;
    
    int platform = [[command.arguments objectAtIndex:0] intValue];//0-微信朋友圈，1-微信好友，2-新浪微博，3-QQ空间，4-QQ好友
    if (platform < 0 || platform > 4) return;
    NSString* text = [command.arguments objectAtIndex:1];
    NSString* imgUrl = [command.arguments objectAtIndex:2];
    NSString* title = [command.arguments objectAtIndex:3];
    NSString* url = [command.arguments objectAtIndex:4];
    int sendMode = [[command.arguments objectAtIndex:5] intValue];//0-通常发送，1-静默发送
    
    UIImage* image = [UIImage imageWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:imgUrl]]];
    NSString* platformName = [@[UMShareToWechatTimeline,UMShareToWechatSession,UMShareToSina,UMShareToQzone,UMShareToQQ] objectAtIndex:platform];
    
    switch (platform) {
        case 0:
            [UMSocialData defaultData].extConfig.wechatTimelineData.url = url;
            [UMSocialData defaultData].extConfig.wechatTimelineData.title = title;
            break;
            
        case 1:
            [UMSocialData defaultData].extConfig.wechatSessionData.url = url;
            [UMSocialData defaultData].extConfig.wechatSessionData.title = title;
            [UMSocialData defaultData].extConfig.wechatSessionData.shareText = text;
            break;
            
        case 2:
            [UMSocialData defaultData].extConfig.sinaData.shareText = text;
            break;
            
        case 3:
            [UMSocialData defaultData].extConfig.qzoneData.url = url;
            [UMSocialData defaultData].extConfig.qzoneData.title = title;
            [UMSocialData defaultData].extConfig.qzoneData.shareText = text;
            break;
            
        case 4:
            [UMSocialData defaultData].extConfig.qqData.url = url;
            [UMSocialData defaultData].extConfig.qqData.title = title;
            [UMSocialData defaultData].extConfig.qqData.shareText = text;
            break;
            
        default:
            return;
            break;
    }
    if (sendMode) {
        [[UMSocialDataService defaultDataService] postSNSWithTypes:@[platformName]
                                                           content:@""
                                                             image:image
                                                          location:nil
                                                       urlResource:nil
                                               presentedController:self.viewController
                                                        completion:^(UMSocialResponseEntity *shareResponse){
                                                            CDVPluginResult* pluginResult;
                                                            if (shareResponse.responseCode == UMSResponseCodeSuccess){
                                                                pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
                                                            } else {
                                                                pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
                                                            }
                                                            [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId_share];
                                                        }];
    } else {
        [[UMSocialControllerService defaultControllerService] setShareText:@""
                                                                shareImage:image
                                                          socialUIDelegate:self];
        [UMSocialSnsPlatformManager getSocialPlatformWithName:platformName].snsClickHandler(self.viewController,[UMSocialControllerService defaultControllerService],YES);
    }
}

-(void)didFinishGetUMSocialDataInViewController:(UMSocialResponseEntity *)response
{
    CDVPluginResult* pluginResult;
    if (response.responseCode == UMSResponseCodeSuccess){
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    } else {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
    }
    [self.commandDelegate sendPluginResult:pluginResult callbackId:self.currentCallbackId_share];
}

- (void)openLoginBox:(CDVInvokedUrlCommand*)command
{
    self.currentCallbackId_login = command.callbackId;
    __weak __typeof(&*self)me = self;
    
    id<ALBBOpenAccountUIService> service = [[ALBBSDK sharedInstance] getService:@protocol(ALBBOpenAccountUIService)];
    [service presentLoginViewController:self.viewController success:^(ALBBOpenAccountSession *currentSession) {
        //NSLog([NSString stringWithFormat:@"token：%@ \n user：%@", [currentSession getAuthToken], [currentSession getUser]]);
        NSDictionary* loginResult = @{
                                      @"type": @"phone",
                                      @"token": [currentSession getAuthToken],
                                      @"account": [currentSession getUser].accountId,
                                      };
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:loginResult];
        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId_login];
    } failure:^(NSError *error) {
        //NSLog([NSString stringWithFormat:@"error:%@", error]);
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId_login];
    }ssoCallback:^(ALBBSSOResponseEntity *response, ALBBOpenAccountSession *session) {
        //如果没有集成三方登录SDK ，这里回调不需要处理
        //NSLog([NSString stringWithFormat:@"code: %u \n message: %@ \n data: %@ \n user:%@", response.responseCode, response.message, response.data,[session getUser]]);
        NSDictionary* loginResult = @{
                                      @"type": @"oauth",
                                      @"token": [session getAuthToken],
                                      @"account": [session getUser].accountId,
                                      @"data": response.data,
                                      };
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:loginResult];
        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId_login];
    }];
}

#pragma mark 注册苹果的推送
-(void)registerAPNS
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onAppLaunch:)
                                                 name:@"UIApplicationDidFinishLaunchingNotification" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onDeviceTokenReceive:)
                                                 name:@"CDVRemoteNotification" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onRemoteReceive:)
                                                 name:@"CDVRemoteReceiveNotification" object:nil];
}
-(void)onAppLaunch:(NSNotification *)notification
{
    NSLog(@"on app launch");
    UIApplication* app = [UIApplication sharedApplication];
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {// iOS 8 Notifications
        [app registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:(UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge) categories:nil]];
        [app registerForRemoteNotifications];
    } else {// iOS < 8 Notifications
        [app registerForRemoteNotificationTypes:(UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound)];
    }
    if (notification) {
        NSDictionary *launchOptions = [notification userInfo];
        if (launchOptions) {
            //NSLog(@"!!!!!!!!!!!!!!!!! has notification !!!!!!!!!!!!!!!!!");
            [CloudPushSDK handleLaunching:launchOptions];// 作为 apns 消息统计
        }
    }
}
// 苹果推送服务回调，注册 deviceToken
- (void)onDeviceTokenReceive:(NSNotification *)notification
{
    NSData* deviceToken = [notification object];
    NSLog(@"device token : %@", deviceToken);
    [CloudPushSDK registerDevice:deviceToken];
    NSLog(@"************************* cloud push deviceId : %@", [CloudPushSDK getDeviceId]);
}
// 通知统计回调，统计打开率
- (void)onRemoteReceive:(NSNotification *)notification
{
    NSDictionary* userInfo = [notification object];
    NSLog(@"user info : %@", userInfo);
    [self.commandDelegate evalJs:@"Taobao.fireNotificationReceive();"];
    [CloudPushSDK handleReceiveRemoteNotification:userInfo];
}

- (void)handleOpenURL:(NSNotification*)notification
{
    // override to handle urls sent to your app
    // register your url schemes in your App-Info.plist
    
    NSURL* url = [notification object];
    
    if ([url isKindOfClass:[NSURL class]]) {
        //URL是否已经被TAE处理过
        [[ALBBSDK sharedInstance] handleOpenURL:url];
    }
}

- (void)useNativeTaobao:(CDVInvokedUrlCommand*)command
{
    id value = [command.arguments objectAtIndex:0];
    if (value != [NSNull null]) {
        [[ALBBSDK sharedInstance] setUseTaobaoNativeDetail:[value boolValue]];
    }
}
- (void)logout:(CDVInvokedUrlCommand*)command
{
    [_loginService logout];
    CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
}

- (void)showLogin:(CDVInvokedUrlCommand*)command
{
    self.currentCallbackId = command.callbackId;
    __weak __typeof(&*self)me = self;

    if(![[TaeSession sharedInstance] isLogin]){
        [_loginService showLogin:self.viewController
                 successCallback:^(TaeSession *session){
                        //NSLog(@"%@", [NSString stringWithFormat:@"登录的用户信息:%@,登录时间:%@",[session getUser],[session getLoginTime]]);
                        TaeUser* user = [session getUser];
                        NSDictionary* ui = @{
                                             @"id": user.userId,
                                             @"nick": user.nick,
                                             @"avatarUrl": user.iconUrl,
                                             };
                        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:ui];
                        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId];
                } failedCallback:^(NSError *error){
                        NSLog(@"登录失败");
                        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR];
                        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId];
        }];
    } else {
        TaeSession *session = [TaeSession sharedInstance];
        //NSLog(@"%@", [NSString stringWithFormat:@"登录的用户信息:%@,登录时间:%@",[session getUser],[session getLoginTime]]);
        TaeUser* user = [session getUser];
        NSDictionary* ui = @{
                             @"id": user.userId,
                             @"nick": user.nick,
                             @"avatarUrl": user.iconUrl,
                             };
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:ui];
        [me.commandDelegate sendPluginResult:pluginResult callbackId:me.currentCallbackId];
    }
}

- (void)showItemDetailByItemId:(CDVInvokedUrlCommand*)command
{
    NSNumber* itemId;
    int itemType = 1;
    id arg0 = [command.arguments objectAtIndex:0];
    id arg1 = [command.arguments objectAtIndex:1];
    
    if ([arg0 isKindOfClass:[NSNumber class]])
    {
        itemId = arg0;
    }
    else if ([arg0 isKindOfClass:[NSString class]])
    {
        itemId = [[[NSNumberFormatter alloc]init] numberFromString:arg0];
    }
    
    if ([arg1 isKindOfClass:[NSNumber class]])
    {
        itemType = [arg1 intValue];
    }
    else if ([arg1 isKindOfClass:[NSString class]])
    {
        itemType = [arg1 intValue];
    }
    
    self.currentCallbackId = command.callbackId;
    //[pluginResult setKeepCallback:[NSNumber numberWithBool:YES]];

    if (itemId == nil) {
        CDVPluginResult* pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"incorrect number of arguments"];
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
    } else {
        //[self.commandDelegate runInBackground:^{
            NSDictionary* params = @{@"isv_code":@"99jun-app"};// isv_code
            [_itemService showItemDetailByItemId:self.viewController
                                      isNeedPush:NO
                               webViewUISettings:_uiSettings
                                          itemId:itemId
                                        itemType:itemType
                                          params:params
                     tradeProcessSuccessCallback:_tradeProcessSuccessCallback
                      tradeProcessFailedCallback:_tradeProcessFailedCallback];

        //}];
    }
}

- (void)showTaoKeItemDetailByItemId:(CDVInvokedUrlCommand*)command
{
    NSNumber* itemId;
    int itemType = 1;
    id arg0 = [command.arguments objectAtIndex:0];
    id arg1 = [command.arguments objectAtIndex:1];
    
    if ([arg0 isKindOfClass:[NSNumber class]])
    {
        itemId = arg0;
    }
    else if ([arg0 isKindOfClass:[NSString class]])
    {
        itemId = [[[NSNumberFormatter alloc]init] numberFromString:arg0];
    }
    
    if ([arg1 isKindOfClass:[NSNumber class]])
    {
        itemType = [arg1 intValue];
    }
    else if ([arg1 isKindOfClass:[NSString class]])
    {
        itemType = [arg1 intValue];
    }
    
    self.currentCallbackId = command.callbackId;

    if (itemId == nil) {
        CDVPluginResult* pluginResult;
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"incorrect number of arguments"];
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
    } else {
        //[self.commandDelegate runInBackground:^{
            NSDictionary* params = @{@"isv_code":@"99jun-app-taoke"};// isv_code
            [_itemService showTaoKeItemDetailByItemId:self.viewController
                                           isNeedPush:NO
                                    webViewUISettings:_uiSettings
                                               itemId:itemId
                                             itemType:itemType
                                               params:params
                                          taoKeParams:_taoKeParams
                          tradeProcessSuccessCallback:_tradeProcessSuccessCallback
                           tradeProcessFailedCallback:_tradeProcessFailedCallback];
        //}];
    }
}

- (void)showCart:(CDVInvokedUrlCommand*)command
{
    self.currentCallbackId = command.callbackId;
    //[self.commandDelegate runInBackground:^{
        ALBBTradePage *page=[ALBBTradePage myCartsPage];
        [_tradeService  show:self.viewController
                  isNeedPush:YES
           webViewUISettings:_uiSettings
                        page:page
                 taoKeParams:_taoKeParams
 tradeProcessSuccessCallback:_tradeProcessSuccessCallback
  tradeProcessFailedCallback:_tradeProcessFailedCallback];
    //}];
}

- (void)showOrder:(CDVInvokedUrlCommand*)command
{
    self.currentCallbackId = command.callbackId;
    //[self.commandDelegate runInBackground:^{
        ALBBTradePage *page=[ALBBTradePage myOrdersPage:0 isAllOrder:1];
        [_tradeService  show:self.viewController
                  isNeedPush:YES
           webViewUISettings:_uiSettings
                        page:page
                 taoKeParams:_taoKeParams
 tradeProcessSuccessCallback:_tradeProcessSuccessCallback
  tradeProcessFailedCallback:_tradeProcessFailedCallback];
    //}];
}

- (void)showPage:(CDVInvokedUrlCommand*)command
{
    NSString* pageUrl = [command argumentAtIndex:0];
    
    self.currentCallbackId = command.callbackId;
    
    if (pageUrl != nil) {
            [_itemService showPage:self.viewController
                        isNeedPush:NO
                           pageUrl:pageUrl
                 webViewUISettings:_uiSettings
       tradeProcessSuccessCallback:_tradeProcessSuccessCallback
        tradeProcessFailedCallback:_tradeProcessFailedCallback];
    } else {
        CDVPluginResult* pluginResult;
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"incorrect number of arguments"];
        [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
    }
}


#pragma mark 注册接收CloudChannel 推送下来的消息
- (void)registerMsgReceive {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onMessageReceived:)
                                                 name:@"CCPDidReceiveMessageNotification"
                                               object:nil]; // 注册
}
// 推送下来的消息抵达的处理
- (void)onMessageReceived:(NSNotification *)notification {
    NSData* data = [notification object];
    NSString *message = [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
    NSLog(@"%@", message);
}


@end
