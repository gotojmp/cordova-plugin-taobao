//
//  WXOImageBubbleChatView.h
//  Messenger
//
//  Created by muqiao.hyk on 13-4-19.
//
//

#import "YWBaseBubbleChatView.h"

@interface WXOImageBubbleChatView : YWBaseBubbleChatView<YWBaseBubbleChatViewInf>

- (void)setProgressViewHidden:(BOOL)aHidden;
- (void)setProgress:(CGFloat)aProgress;


@property (nonatomic, strong) UIImageView *msgImageView;
@property (nonatomic, assign) BOOL showMask;

@end