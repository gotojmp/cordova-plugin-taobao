//
//  MultiTextBubbleViewModel.h
//  Messenger
//
//  Created by 慕桥(黄玉坤) on 1/21/15.
//
//

#import "YWBaseBubbleViewModel.h"
#import "BoxMsgLabelNode.h"

@interface WXOMultiTextBubbleViewModel : YWBaseBubbleViewModel

// @[BoxMsgLabelNode, ...]
@property (nonatomic, strong) NSArray *labelNodes;
@end
