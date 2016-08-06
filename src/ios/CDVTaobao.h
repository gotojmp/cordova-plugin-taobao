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

#import <Cordova/CDVPlugin.h>
#import <Cordova/CDVInvokedUrlCommand.h>

#import <ALBBSDK/ALBBSDK.h>
#import <ALBBPush/CloudPushSDK.h>
#import <ALBBPush/CCPSysMessage.h>
#import <ALBBTradeSDK/ALBBTradeService.h>
#import <ALBBTradeSDK/ALBBItemService.h>
#import <ALBBTradeSDK/ALBBCartService.h>
#import <ALBBLoginSDK/ALBBLoginService.h>
#import <ALBBMediaService/ALBBMediaService.h>
#import <ALBBOpenAccount/ALBBOpenAccountSDK.h> //云账号基础SDK
#import <ALBBOpenAccountUI/ALBBOpenAccountUIService.h> //云账号UI服务
#import <ALBBOpenAccountSSO/ALBBOpenAccountSSOSDK.h>


@interface CDVTaobao : CDVPlugin

@property (nonatomic, strong) id<ALBBItemService> itemService;
@property (nonatomic, strong) id<ALBBCartService> cartService;
@property (nonatomic, strong) id<ALBBLoginService> loginService;
@property (nonatomic, strong) id<ALBBTradeService> tradeService;
@property (nonatomic, strong) id<ALBBMediaServiceProtocol> mediaService;
@property (nonatomic, strong) id<ALBBOpenAccountSSOServiceProtocol> ssoService;
@property (nonatomic, strong) ALBBTradeTaokeParams* taoKeParams;
@property (nonatomic, strong) TaeWebViewUISettings* uiSettings;
@property (nonatomic, strong) tradeProcessSuccessCallback tradeProcessSuccessCallback;
@property (nonatomic, strong) tradeProcessFailedCallback tradeProcessFailedCallback;

@property (nonatomic, copy) NSString* currentCallbackId;
@property (nonatomic, copy) NSString* currentCallbackId_share;
@property (nonatomic, copy) NSString* currentCallbackId_login;
@property (nonatomic, copy) NSString* currentCallbackId_media;

- (void)useNativeTaobao:(CDVInvokedUrlCommand*)command;
- (void)logout:(CDVInvokedUrlCommand*)command;
- (void)showLogin:(CDVInvokedUrlCommand*)command;
- (void)showItemDetailByItemId:(CDVInvokedUrlCommand*)command;
- (void)showTaoKeItemDetailByItemId:(CDVInvokedUrlCommand*)command;
- (void)showCart:(CDVInvokedUrlCommand*)command;
- (void)showPage:(CDVInvokedUrlCommand*)command;

@end
