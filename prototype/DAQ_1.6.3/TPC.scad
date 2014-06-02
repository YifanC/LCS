nrings = 8;


parallel=104.202003;
//parallel = 0;

translate(v = [90+75,-105,0]) rotate ([0,15,0]) 
{
	// Wire plane
	translate(v = [0,0, 0]) Wireplane();
	translate(v = [0,2.5+3, 0]) rotate ([0,90,0]) Wireplane();
	// TPC
	Rings(8);
	Rod();
	
}

color("red") intersection() {
	//# translate(v = [150,0,0]) cube([400,2,250],center = true);
	//# rotate ([0,15,0]) translate(v = [150,0,0]) cube([400,400,2],center = true);
}
color("red") rotate ([0,0,25]) translate(v = [150,0,0]) cube([400,2,2],center = true);
//laser(0,-45);
module laser(rotateX, rotateVert) {
	rotate ([0,-90+2*(90-rotateVert),(-rotateX+parallel+180)]) union() {
		rotate([0,-90,0]){
			//cylinder (h =8, r=25.4/2, center = true);
			color([0,1,0]) cylinder (h = 300, r=3, center = false);
		}
	}
}

module Wireplane() {
rotate ([90,0,0]) 
	color("OrangeRed") difference() {
		cylinder (h=2.5, r=210/2, center = true);
   		intersection() {
			cube (130, r=90-18, center = true);
			rotate([0,0,45])  cube ([140,140,15], center = true);
		}
	}
	cylinder (h = 142, r=0.1, center = true); // cable
}

module Rings(nrings) {
	color("Orange") translate(v = [0, 8+23, 0])
	for (i = [0 : nrings-1]){
			translate(v = [0, i*23, 0]) {
			rotate ([90,0,0]) difference() {
				cylinder (h = 3, r=90, center = true);	
				cylinder (h = 10, r=90-18, center = true);
			}
		}
	}
	// Cathode
	color("DarkGoldenrod") translate(v = [0, 8+(nrings+1)*23, 0]) rotate ([90,0,0]) difference() {
		cylinder (h = 3, r=90, center = true);
		rotate ([0,0,-45.4+15]) translate(v = [90-45.4, 0, 0]) cylinder (h = 10, r=6.5, center = true);
		rotate ([0,0,180-40.5+15]) translate(v = [90-40.5, 0, 0]) cylinder (h = 10, r=6.5, center = true);
	}
}

module Rod() {
	for (i = [0 : 2]){
		rotate ([0,-45+i*120,0]){
			translate(v = [80,220, 0]) rotate ([90,0,0]) #cylinder (h=23*10, r=4, center = false);
		}
	}
}

//laser(-45,35);