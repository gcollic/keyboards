// Use openSCAD Nightly
// with `Manifold` engine

$fn = 140;

PMW_PCB_WIDTH = 21;
PMW_PCB_HEIGHT = 28;
SENSOR_LENS_HEIGHT_ABOVE_BOARD = 3.5;
SENSOR_LENS_WIDTH = 21; 
SENSOR_LENS_HEIGHT = 22;
SENSOR_PCB_THICKNESS = 1.5;

STANDOFF_HEIGHT = 6;

lensReferenceSurfaceToBallSurface = 2.3; // 2.2~2.6
ballSurfaceToPcb = SENSOR_LENS_HEIGHT_ABOVE_BOARD+ lensReferenceSurfaceToBallSurface;
ballDiameter = 25.4;
ballCenterToPcb = ballDiameter/2 + ballSurfaceToPcb;
pcbMargin = 1.2;
pcbCoverMargin = 2;
pcbFullClearance = 4;//5;
angle = 76;
marginCausedByAngle = 2;
m2nutHoleWidth = 4.2;
screwApart = 24.2;
screwHole = 3;
// M2 insert officially 3.2, and 5.8 for the matter around
m2InsertD = 3.4;
m2InsertL = 5;
m2Hole = 2.1;
ground = -10.51;
mainRingDiameter = 33.1;
ringRadius = 2;
ringHeight = 6.5;
TRACKBALL_BEARING_MOUNT_CLEARANCE = 1.2;

epsilon = 0.01;

module squircle(
  size = [1,1],
  radius = 1){
  hull()
    for (dirx =[-1,1])
      for (diry =[-1,1])
        translate([dirx*(size[0]/2 - radius), diry*(size[1]/2 - radius)])
          circle(radius);
}

module ball(){
    sphere(d = ballDiameter);
}


module chamferedCylinder(diameter, height, chamfer){
  rotate_extrude(convexity = 10)
    union(){
      translate([diameter/2-chamfer, height-chamfer])
        circle(r = chamfer);
      square([diameter/2, height-chamfer]);
      square([diameter/2-chamfer, height]);
    }
}

module bearingBallsHoles(){
  bearingOffsetXY = 12.853;
  BEARING_DIAMETER = 3.175;
  BEARING_OFFSET_Z = 3.8;
  BEARING_TOLERENCE = 0.05;
  for (i = [0:2]) {
    angle = 120 * i;
    translate([cos(angle), sin(angle), 0] * bearingOffsetXY)
      rotate([0, 0, angle])
        translate([0,0, BEARING_OFFSET_Z]){
          sphere(d = BEARING_DIAMETER + BEARING_TOLERENCE * 2);
          // Uncomment to see the in-place ghost of the bearings
          // %sphere(d = BEARING_DIAMETER, $fn=60);
        };
  }
}
module lens(){
  linear_extrude(STANDOFF_HEIGHT)
    squircle(
      [SENSOR_LENS_WIDTH+pcbCoverMargin, SENSOR_LENS_HEIGHT+pcbCoverMargin],
      radius = 4
    );
}
module pmwCoverPositive(){
  linear_extrude(epsilon)
    squircle(
      [
        PMW_PCB_WIDTH+pcbMargin+pcbCoverMargin*2,
        PMW_PCB_HEIGHT+pcbMargin+pcbCoverMargin*2
      ],
      radius = 3
    );
  linear_extrude(STANDOFF_HEIGHT)
    squircle(
      [SENSOR_LENS_WIDTH+pcbCoverMargin, SENSOR_LENS_HEIGHT+pcbCoverMargin],
      radius = 4
    );
}

module screwHole()
{
  cylinder(d=screwHole, h=STANDOFF_HEIGHT);
  translate([0, 0, SENSOR_LENS_HEIGHT_ABOVE_BOARD*0.75])
    cube([m2nutHoleWidth, m2nutHoleWidth*1.5, SENSOR_LENS_HEIGHT_ABOVE_BOARD/2], center=true);
}

module pmwCoverNegative()
{
  linear_extrude(STANDOFF_HEIGHT+ballDiameter/2) union()
  {
    // sensor channel
    SENSOR_CHANNEL_WIDTH = 12;
    SENSOR_CHANNEL_HEIGHT = 15;
    squircle(
      size = [SENSOR_CHANNEL_WIDTH, SENSOR_CHANNEL_HEIGHT],
      radius = 1);
  }
  // cutout for sensor
  linear_extrude(SENSOR_LENS_HEIGHT_ABOVE_BOARD + epsilon)
    squircle(
      size = [SENSOR_LENS_WIDTH, SENSOR_LENS_HEIGHT],
      radius = 4);
  // cutout for pcb
  translate([0, 0, epsilon - pcbFullClearance*0.5])
    cube([PMW_PCB_WIDTH+pcbMargin, PMW_PCB_HEIGHT+pcbMargin, pcbFullClearance], center = true);
  // screws
  for (direction =[-1,1])
    translate([0, direction* screwApart/2, 0])
      screwHole();
}

module movePcbToPosition()
{
  rotate([0,angle,0])
    translate([0, 0, -ballCenterToPcb])
    children();
}

module pcb(){
  color([0,0.8,0, 0.2])
    translate([0, 0, -SENSOR_PCB_THICKNESS/2])
    cube([PMW_PCB_WIDTH, PMW_PCB_HEIGHT, SENSOR_PCB_THICKNESS], center = true);
}
module pcbClearance(){
  color([0,0.8,0])
    translate([0, 0, -pcbFullClearance/2])
    cube([PMW_PCB_WIDTH, PMW_PCB_HEIGHT, pcbFullClearance], center = true);
}

module m2InsertHole()
{
  cylinder(h=m2InsertL, d=m2InsertD);
}
module m2Hole()
{
  cylinder(d=m2Hole);
}
module m2InsertPosition()
{
  for (direction =[-1,1])
    translate([-11, direction* 13.2, -epsilon])
      children();
  translate([12.4, 6, -0.8-epsilon])
    children();
}

module mainRing()
{
  translate([0,0,ground])
    chamferedCylinder(
      mainRingDiameter,
      ringHeight,
      ringRadius);
}

module halfSquircle(x, y, radius)
{
  module xCenteredSquare(width, height)
  {
    translate([0, -width/2])
      square([height, width]);
  }
  if(radius>0)
    for (direction =[-1,1])
      translate([y-radius, direction*(x/2-radius)])
        circle(radius);
  xCenteredSquare(x, y-radius);
  xCenteredSquare(x- 2 * radius, y);
}

module box(radius=0, wMargin=0, hMargin=0, depth=boxDepth)
{
  linear_extrude(depth){
      halfSquircle(
        PMW_PCB_HEIGHT+pcbMargin+wMargin*2,
        boxHeight+hMargin,
        radius
      );
  }
}

module rail()
{
  for (i =[0.5,1.5])
    translate([0,0,-i*doorDepth])
      difference(){
        box(depth=doorDepth*0.5);
        translate([0,0,-epsilon])
          box(depth=2*epsilon+doorDepth*0.5, wMargin=-doorRailSize);
      }
}

boxHeight = 16.8;
boxStartDistanceFromBallCenter = 20.5;
boxDepth = 5;
doorDepth = 1.5;
doorRailSize = 1;

module door(){
  doorMargin = 0.3;
  box(wMargin=-doorRailSize-doorMargin/2,hMargin=-0.3, depth=doorDepth);
  box(wMargin=-doorMargin, hMargin=-0.3, depth=doorDepth/2-doorMargin);
}

module boxPosition()
{
  translate([-boxStartDistanceFromBallCenter, 0, ground])
    rotate([0,-90,0])
    children();
}

module main()
{
  difference()
  {
    hull(){
      mainRing();
      boxPosition()
        box(3, pcbCoverMargin, pcbCoverMargin);
      movePcbToPosition()
        pmwCoverPositive();
    }
    translate([-epsilon, 0, 0])
      boxPosition(){
        difference(){
          box();
          translate([0,0,boxDepth])
            rail();
        }
      };
    sphere(d = ballDiameter + TRACKBALL_BEARING_MOUNT_CLEARANCE);
    rotate([0, 12, 0])
      translate([0, 0, 8.4 -ballCenterToPcb]){
        bearingBallsHoles();
    }
    movePcbToPosition()
      translate([0, 0, -epsilon])
        pmwCoverNegative();
   
    translate([0, 0, ground])
      union() {
        translate([0, 0, -50])
          cube([100, 100, 100], center=true);
        m2InsertPosition()
          m2InsertHole();
      };
    // Uncomment for a cut view
    // translate([0,-44.5,0])cube([100,100,100], center=true);
  }
}

module plate(){
  difference()
  {
    projection(false) main();
    projection(false)
      union(){
        movePcbToPosition()
        {
          pcb();
          pcbClearance();
          lens();
        };
        intersection()
        {
          ball();
          translate([0, 0, ground-50])
            cube([100, 100, 100], center=true);
        }
        m2InsertPosition() m2Hole();
    }
  }
}

main(); // main 3D model
//%ball(); // Uncomment to see the in-place ghost of the ball
//movePcbToPosition() pcb(); // Uncomment to see the in-place ghost of the pcb
//door(); // sliding door/wall 3D model
//plate(); // 2D cutout for the plate below the PMW holder
