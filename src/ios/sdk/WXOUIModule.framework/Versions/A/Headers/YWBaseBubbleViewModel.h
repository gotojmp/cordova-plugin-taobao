//
//  YWBaseBubbleViewModel.h
//  Messenger
//
//  Created by 慕桥(黄玉坤) on 12/3/14.
//
//

#import <Foundation/Foundation.h>
#import "YWBaseBubbleChatView.h"
#import "YWIMKit.h"

@interface YWBaseBubbleViewModel : NSObject
{
    NSUInteger _layout;
    NSData *_backgroundImage;
    NSData *_highLightBGImage;
}

/// 期望展示的气泡样式
@property (nonatomic, assign) BubbleStyle bubbleStyle;

/// 消息内容供应方，用于显示“消息来源”
@property (nonatomic, strong) NSString *ownerName;

/// 消息长按后弹出菜单项 @[UIMenuItem,...]。你的ViewModel中需要实现这些UIMenuItem的Selector。
@property (nonatomic, strong) NSArray *menuItems;

/// ViewModel对应的bubbleView
@property (nonatomic, weak) YWBaseBubbleChatView *bubbleView;

/// ViewModel对应的imkit，用来响应点击事件
@property (nonatomic, weak) YWIMKit *imkit;

@end

/**
 *  气泡的布局
 */
typedef enum : NSUInteger {
    /// 居中
    WXOBubbleLayoutCenter,
    /// 居左
    WXOBubbleLayoutLeft,
    /// 居右
    WXOBubbleLayoutRight,
} WXOBubbleLayout;

/**
 *  如果BubbleStyle等于BubbleStyleCustomize时，你可以通过设置以下属性，来控制气泡的自定义显示
 */
@interface YWBaseBubbleViewModel(Customize)
/// 自定义气泡的布局方式(左|中|右)
@property (nonatomic, assign) WXOBubbleLayout layout;

/// 自定义气泡背景
@property (nonatomic, strong) NSData *backgroundImage;

/// 自定义高亮气泡背景
@property (nonatomic, strong) NSData *highLightBGImage;

@end


