square = 179;
num = 60;
thickness = 3;
steps = 8;
radius = 20;

module bloc() {
    intersection() {
        rotate(a=7,v=[0,1,0])
        mirrorz()
        linear_extrude(height=140, scale=0.75) {
            difference() {
                rounded_square(square,radius,0.5);
                rounded_square(square-thickness*2,radius-thickness,1);
                //holes();
            }
        }
        cube([300,300,90], center=true); // height
    }
}
//translate([0,0,80-26])
//rounded_square(square, radius, thickness);
difference() {
    bloc();
    holes();
}

module holes() {
    union() {
        mirrorz()
        for (i = [0:steps]) {
            translate([0,0,i*5])
            rotate(a=180/num*i, v=[0,0,1])
            for (j = [0:num]) {
                rotate(a=j*360/num,v=[0,0,1])
                cone(i);          
            }
        }
    }
}

module rounded_square(h,r,z) {
    minkowski() {
        square([h-2*r,h-2*r], center=true);
        circle(r=r, center=true, $fn=50);
    } 
}
module cone(i) {
    rotate(a=90,v=[1,0,0]) cylinder(r1=12,r2=1,h=110-3.5*i, $fn=6);
}

module mirrorz() {
    mirror([0,0,1]) children();
    children();
}
