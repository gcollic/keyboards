// Use openSCAD Nightly
// with `Manifold` engine

controllerHeight = 5.35;
controllerBorderWidth=0.6;
controllerBorderHeight=0.7;
margin = 0.2;
trs_height = 5.6;
minFirstLayer = 0.4;
height = max(controllerHeight,trs_height)+minFirstLayer;
m2InsertL = 5;
notchHeight = 1.4;
sideNotch=0.4;
sideDepthMargin=0.8;
sideDepth=1;

epsilon = 0.01;
$fn=120;


module importPart(part)
{
  import("controller_cover.svg", layer=part);
}

module slice(z, h){
  translate([0,0,z])
    linear_extrude(height=h)
    children();
}

module cutBottom(h){
  slice(-epsilon, h+epsilon)
    children();
}

module trs(){
  cutBottom(trs_height)
    offset(margin)
    importPart("trs");
}

module controller(){
  cutBottom(controllerHeight)
    offset(margin)
    importPart("controller");
}

module controllerBorder(){
  slice(controllerHeight-controllerBorderHeight,controllerBorderHeight)
    difference(){
      intersection(){
        importPart("main");
        offset(margin)
          importPart("controller");
      }
      translate([-controllerBorderWidth,0,0])
        offset(margin)
          importPart("controller");
    }
}

module main(){
  slice(0, height)
    importPart("main");
}

module screws(){
  slice(-epsilon, m2InsertL+epsilon)
    importPart("screws");
}

module sidePosition(){
  translate([0,47.157,0])
    rotate([90, 0, 0])
      children();
}

module sidePositive(){
  sidePosition()
    union(){
      slice(-sideDepthMargin, sideDepth)
        importPart("side_ref");
      slice(0, sideNotch+sideDepth)
        importPart("side_notch");
    };
}

module sideNegative(){
  sidePosition()
    slice(-sideDepthMargin-epsilon, sideNotch+sideDepth+sideDepthMargin+2*epsilon)
      offset(0.3)
      importPart("side_hole");
}

module whole(){
  difference(){
    union(){
      hull(){
        main();
        sidePositive();
      }
      translate([8,-0.2,0])
        cube([8, 0.2, notchHeight]);
    }
    screws();
    trs();
    controller();
    sideNegative();
  };
  controllerBorder();
  difference(){
    sidePositive();
    sideNegative();
  };
}

whole();
