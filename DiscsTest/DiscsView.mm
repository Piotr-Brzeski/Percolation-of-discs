//
//  DiscsView.mm
//  DiscsTest
//
//  Created by Piotr Brzeski on 2021-05-26.
//

#import "DiscsView.h"
#include "../discs/system.h"
#include <algorithm>
#include <iostream>

namespace {

constexpr auto disc_radius = float_type(0.1);

System discsSystem(disc_radius);

void printEdgeClusters() {
  std::cout << "Edge clusters:" << std::endl;
  auto& edge_clusters = discsSystem.get_edge_clusters();
  for(auto cluster : edge_clusters) {
    std::cout << cluster.first << " : ";
    for(auto angle : cluster.second) {
      std::cout << angle << ", ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void printPercolations() {
  auto& percolation_probabilities = discsSystem.get_percolation_probabilities();
  for(auto [number_of_discs, percolation_probability] : percolation_probabilities) {
    std::cout << number_of_discs << '\t' << percolation_probability << "\n";
  }
}

} //  namespace

@implementation DiscsView

- (CGFloat)scale {
  CGFloat size = std::min(self.frame.size.width, self.frame.size.height);
  CGFloat scale = size/(1 + 2*disc_radius);
  return scale;
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];
  CGFloat scale = self.scale;
  NSAffineTransform *transform = [[NSAffineTransform alloc] init];
  [transform translateXBy:self.frame.size.width yBy:self.frame.size.height];
  [transform set];
  [NSColor.grayColor set];
  auto& discs = discsSystem.get_discs();
  for(auto disc : discs) {
    auto& position = disc.get_position();
    CGFloat x = scale*position.x;
    CGFloat y = scale*position.y;
    [self drawCircleWithRadius:scale*disc_radius x:x y:y];
  }
  
  [NSColor.blackColor set];
  [self drawCircleWithRadius:scale x:0 y:0];
  
  [NSColor.blueColor set];
  {
    NSBezierPath* path = [[NSBezierPath alloc] init];
    [path moveToPoint:NSMakePoint(0, 0)];
    [path lineToPoint:NSMakePoint(scale, 0)];
    NSAffineTransform* rotation = [[NSAffineTransform alloc] init];
    [rotation rotateByRadians:2*M_PI/3];
    [rotation concat];
    [path stroke];
    [rotation concat];
    [path stroke];
    [rotation concat];
    [path stroke];
  }
  
  [NSColor.redColor set];
  auto& main_cluster = discsSystem.get_main_edge_cluster();
  for(auto angle : main_cluster) {
    NSAffineTransform* rotation = [[NSAffineTransform alloc] init];
    [rotation rotateByRadians:angle];
    [rotation concat];
    
    NSBezierPath* path = [[NSBezierPath alloc] init];
    [path moveToPoint:NSMakePoint(scale - 5, 0)];
    [path lineToPoint:NSMakePoint(scale + 5, 0)];
    [path stroke];
    
    [rotation invert];
    [rotation concat];
  }
}

- (void)drawCircleWithRadius:(CGFloat)radius x:(CGFloat)x y:(CGFloat)y {
  CGFloat diameter = 2*radius;
  NSRect rect = NSMakeRect(x - radius, y - radius, diameter, diameter);
  [[NSBezierPath bezierPathWithOvalInRect:rect] stroke];
}

- (BOOL)addDisc {
  discsSystem.add_disc();
  [self setNeedsDisplay:YES];
  return self.isDone;
}

- (BOOL)addDiscAt:(NSPoint)location {
  CGFloat x = location.x - self.frame.size.width/2;
  CGFloat y = location.y - self.frame.size.height/2;
  CGFloat scale = self.scale/2;
  x /= scale;
  y /= scale;
  discsSystem.add_disc(static_cast<float_type>(x), static_cast<float_type>(y));
  [self setNeedsDisplay:YES];
  return self.isDone;
}

- (BOOL)isDone {
  printEdgeClusters();
  auto done = discsSystem.is_done();
  if(done) {
    printPercolations();
  }
  return done;
}

@end
