//
//  DiscsView.h
//  DiscsTest
//
//  Created by Piotr Brzeski on 2021-05-26.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface DiscsView : NSView

- (BOOL)addDisc;
- (BOOL)addDiscAt:(NSPoint)location;

@end

NS_ASSUME_NONNULL_END
