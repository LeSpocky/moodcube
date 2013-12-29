// outer wall made from plexiglass to form a cube 150mm×150mm

module wand() {
	linear_extrude( height = 6 ) 
			polygon( points = [ [6,0], [6,25], [0,25], [0,50], [6,50], [6,75], 
				[0,75], [0,100], [6,100], [6,125], [0,125], [0,150], [144,150], 
				[144,125], [150,125], [150,100], [144,100], [144,75], [150,75], 
				[150,50], [144,50], [144,25], [150,25], [150,0] ] );
}

module wand_1() {
	difference() {
		wand();
		translate( [ 65, 5.9, 3 ] ) cube( size = [ 20, 3.2, 4 ] );
		translate ( [ 75, 25, 3 ] ) cylinder( h = 10, r = 4.5, center = true );
	}
}

module wand_2() {
	difference() {
		wand();
		translate( [ 65, 5.9, 3 ] ) cube( size = [ 20, 3.2, 4 ] );
		translate( [ 50, 25, 3 ] ) cylinder( h = 10, r = 5.5, center = true );
		translate( [ 100, 25, 3 ] ) cylinder( h = 10, r = 10.5, center = true );
	}
}

projection( cut = true ) 
	translate ( [ 0, 0, -4 ] ) {
		wand_2();
		translate( [ 155, 0, 0 ] ) wand_1();
		translate( [ 155, 152, 0 ] ) wand_1();
		translate( [ 0, 152, 0 ] ) wand_1();
	}

/* vim: set noet sts=0 ts=4 sw=4 sr: */
