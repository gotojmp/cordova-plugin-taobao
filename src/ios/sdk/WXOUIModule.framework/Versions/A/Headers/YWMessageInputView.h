//
//  WXOMessageInputView.h
//  testFreeOpenIM
//
//  Created by Jai Chen on 15/1/13.
//  Copyright (c) 2015年 taobao. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YWInputViewPlugin.h"

@class YWIMKit;

#define kWXOMessageInputViewMinRecordLenght        1.0

@interface YWMessageInputView : UIView

/**
 *  输入框文本
 */
@property (nonatomic, readwrite)   NSString *text;

/**
 *  文本选择范围，您可以使用该信息，确保能够在正确的位置修改输入框文本
 */
@property (nonatomic, readwrite) NSRange selectedRange;

/**
 *  more面板容器View，可往里添加自定义pluginView
 *  @note 一般不要直接添加子view，如果希望添加子view，请使用下面的push和pop函数
 */
@property (nonatomic, readonly) UIView  *morePanelContentView;

/**
 *  语音输入功能开关，默认 NO，即默认打开语音输入
 */
@property (nonatomic, assign, readwrite) BOOL disableAudioInput;


@property (nonatomic, readonly) UIViewController *controllerRef;

@property (nonatomic, weak) YWIMKit *imkit;

#pragma mark - plugin

// 加载移除pluginContentView

/**
 *  添加子view
 */
- (void)pushContentViewOfPlugin:(id <YWInputViewPluginProtocol>)plugin;
/**
 *  移除子view
 */
- (void)popContentViewOfPlugin:(id <YWInputViewPluginProtocol>)plugin;

// 往更多面板中添加与删除item

/**
 *  在最后添加新的item
 */
- (void)addPlugin:(id <YWInputViewPluginProtocol>)plugin;

/**
 *  移除某个item
 */
- (void)removePlugin:(id <YWInputViewPluginProtocol>)plugin;

/**
 *  移除所有item，包含前置插件
 */
- (void)removeAllPlugins;

/**
 *  获取plugin列表，包含前置插件
 */
- (NSArray *)allPluginList;

/**
 *  刷新
 */
- (void)reloadPluginData;

/**
 *  激活某个plugin，相当于手动按下这个插件。一般用在希望进入聊天页面就激活某个输入插件的场景
 */
- (void)activatePlugin:(id<YWInputViewPluginProtocol>)plugin;

/**
 * plugin如果需要通知到对方己方当前的输入状态，可以调用下面两个API，告知输入框发送当前用户正在使用该插件
 *
 * plugin进入编辑状态(通知YWMessageInputView更新输入状态)
 */
- (void)pluginWillBeginEdit:(id <YWInputViewPluginProtocol>)plugin;
/**
 *  plugin结束编辑状态(通知YWMessageInputView更新输入状态)
 */
- (void)pluginDidEndEdit:(id <YWInputViewPluginProtocol>)plugin;

@end
