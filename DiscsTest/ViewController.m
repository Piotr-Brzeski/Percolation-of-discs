//
//  ViewController.m
//  DiscsTest
//
//  Created by Piotr Brzeski on 2021-05-26.
//

#import "ViewController.h"
#import "DiscsView.h"

@interface ViewController()

@property (weak) IBOutlet DiscsView* discsView;
@property (weak) IBOutlet NSButton *addButton;

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];

  // Do any additional setup after loading the view.
}

- (void)setRepresentedObject:(id)representedObject {
  [super setRepresentedObject:representedObject];

  // Update the view, if already loaded.
}

- (IBAction)addDisc:(NSButton*)sender {
  BOOL done = [self.discsView addDisc];
  self.addButton.enabled = !done;
}

- (IBAction)click:(NSClickGestureRecognizer *)sender {
  if(self.addButton.enabled) {
    NSPoint location = [sender locationInView:self.discsView];
    BOOL done = [self.discsView addDiscAt:location];
    self.addButton.enabled = !done;
  }
}

@end
