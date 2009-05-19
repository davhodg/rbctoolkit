# RBC.barchart.configure.M.tcl --
#
###Abstract
# This file tests the different configuration options of the barchart BLT component.
#
###Copyright
# See the file "license.terms" for information on usage and
# redistribution of this file, and for a DISCLAIMER OF ALL WARRANTIES.
#
###Revision String
# SVN: $Id$

###Package Definition
package provide barchart.configure

package require rbc
namespace import rbc::*

namespace eval barchart.configure {
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring aspect changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.1.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.1.1.Body {} {
		.barchart1 configure -aspect 2
	}
	
	proc RBC.barchart.configure.M.1.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring background changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.2.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.2.1.Body {} {
		.barchart1 configure -background red
	}
	
	proc RBC.barchart.configure.M.2.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring barmode changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.3.1.Setup {} {
		barchart .barchart1
		.barchart1 bar create Bar1 -x [list 1 2 3 3] -y [list 1 2 3 4]
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.3.1.Body {} {
		.barchart1 configure -barmode aligned
	}
	
	proc RBC.barchart.configure.M.3.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring barwidth changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.4.1.Setup {} {
		barchart .barchart1
		.barchart1 bar create Bar1 -x [list 1 2 3 3] -y [list 1 2 3 4]
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.4.1.Body {} {
		.barchart1 configure -barwidth 80
	}
	
	proc RBC.barchart.configure.M.4.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring baseline changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.5.1.Setup {} {
		barchart .barchart1
		.barchart1 bar create Bar1 -x [list 1 2 3 3] -y [list 1 2 3 4]
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.5.1.Body {} {
		.barchart1 configure -baseline 10000
		.barchart1 bar configure Bar1 -hide yes
		.barchart1 bar configure Bar1 -hide no
	}
	
	proc RBC.barchart.configure.M.5.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring borderwidth changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.6.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.6.1.Body {} {
		.barchart1 configure -borderwidth 1000
	}
	
	proc RBC.barchart.configure.M.6.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring bottommargin changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.7.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.7.1.Body {} {
		.barchart1 configure -bottommargin 2
	}
	
	proc RBC.barchart.configure.M.7.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring bottomvariable changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.8.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.8.1.Body {} {
		.barchart1 configure -bottomvariable left
	}
	
	proc RBC.barchart.configure.M.8.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring cursor changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.9.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.9.1.Body {} {
		.barchart1 configure -cursor circle
	}
	
	proc RBC.barchart.configure.M.9.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring data changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.10.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.10.1.Body {} {
		.barchart1 configure -data {1 2 3 4}
	}
	
	proc RBC.barchart.configure.M.10.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring datacommand changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.11.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.11.1.Body {} {
		.barchart1 configure -datacommand {set x}
	}
	
	proc RBC.barchart.configure.M.11.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring font changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.12.1.Setup {} {
		barchart .barchart1 -title "Test"
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.12.1.Body {} {
		.barchart1 configure -font *-Helvetica-*-R-Normal-*-10-120-*
	}
	
	proc RBC.barchart.configure.M.12.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring foreground changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.13.1.Setup {} {
		barchart .barchart1 -title "Test"
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.13.1.Body {} {
		.barchart1 configure -foreground yellow
	}
	
	proc RBC.barchart.configure.M.13.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring height changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.14.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.14.1.Body {} {
		.barchart1 configure -height 20
	}
	
	proc RBC.barchart.configure.M.14.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring highlightBackground changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.15.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.15.1.Body {} {
		.barchart1 configure -highlightbackground yellow
	}
	
	proc RBC.barchart.configure.M.15.1.Cleanup {} {
		destroy .barchart1
	}
		
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring highlightcolor changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.16.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.16.1.Body {} {
		.barchart1 configure -highlightcolor red
	}
	
	proc RBC.barchart.configure.M.16.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring highlightthickness changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.17.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.17.1.Body {} {
		.barchart1 configure -highlightthickness 10
	}
	
	proc RBC.barchart.configure.M.17.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring invertxy changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.18.1.Setup {} {
		barchart .barchart1
		.barchart1 xaxis configure -title "X Axis"
		.barchart1 yaxis configure -title "Y Axis"
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.18.1.Body {} {
		.barchart1 configure -invertxy true
	}
	
	proc RBC.barchart.configure.M.18.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring justify changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.19.1.Setup {} {
		barchart .barchart1 -title "Test Title is Long \nTitle"
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.19.1.Body {} {
		.barchart1 configure -justify left
	}
	
	proc RBC.barchart.configure.M.19.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring leftmargin changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.20.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.20.1.Body {} {
		.barchart1 configure -leftmargin 10
	}
	
	proc RBC.barchart.configure.M.20.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring leftvariable changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.21.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.21.1.Body {} {
		.barchart1 configure -leftvariable left
	}
	
	proc RBC.barchart.configure.M.21.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring plotbackground changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.22.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.22.1.Body {} {
		.barchart1 configure -plotbackground red
	}
	
	proc RBC.barchart.configure.M.22.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring plotborderwidth changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.23.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.23.1.Body {} {
		.barchart1 configure -plotborderwidth 10
	}
	
	proc RBC.barchart.configure.M.23.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring plotpadx changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.24.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.24.1.Body {} {
		.barchart1 configure -plotpadx 200
	}
	
	proc RBC.barchart.configure.M.24.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring plotpady changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.25.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.25.1.Body {} {
		.barchart1 configure -plotpady 200
	}
	
	proc RBC.barchart.configure.M.25.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring plotrelief changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.26.1.Setup {} {
		barchart .barchart1 -plotborderwidth 2
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.26.1.Body {} {
		.barchart1 configure -plotrelief raised
	}
	
	proc RBC.barchart.configure.M.26.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring relief changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.27.1.Setup {} {
		barchart .barchart1 -borderwidth 2 -plotborderwidth 2
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.27.1.Body {} {
		.barchart1 configure -relief raised
	}
	
	proc RBC.barchart.configure.M.27.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring rightmargin changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.28.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.28.1.Body {} {
		.barchart1 configure -rightmargin 10
	}
	
	proc RBC.barchart.configure.M.28.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring rightvariable changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.29.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.29.1.Body {} {
		.barchart1 configure -rightvariable right
	}
	
	proc RBC.barchart.configure.M.29.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring shadow changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.30.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.30.1.Body {} {
		.barchart1 configure -shadow red
	}
	
	proc RBC.barchart.configure.M.30.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring takefocus changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.31.1.Setup {} {
		barchart .barchart1
		button .button1 -text "Test"
		pack .barchart1
		pack .button1
	}
	
	proc RBC.barchart.configure.M.31.1.Body {} {
		.barchart1 configure -takefocus 1
	}
	
	proc RBC.barchart.configure.M.31.1.Cleanup {} {
		destroy .barchart1 .button1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring tile changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.32.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.32.1.Body {} {
		.barchart1 configure -tile [image create photo -file "buckskin.gif"]
	}
	
	proc RBC.barchart.configure.M.32.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring title changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.33.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.33.1.Body {} {
		.barchart1 configure -title "Test"
	}
	
	proc RBC.barchart.configure.M.33.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring topmargin changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.34.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.34.1.Body {} {
		.barchart1 configure -topmargin 20
	}
	
	proc RBC.barchart.configure.M.34.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring topvariable changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.35.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.35.1.Body {} {
		.barchart1 configure -topvariable top
	}
	
	proc RBC.barchart.configure.M.35.1.Cleanup {} {
		destroy .barchart1
	}
	
	# ------------------------------------------------------------------------------------
	# Purpose: Ensure configuring width changes on screen
	# ------------------------------------------------------------------------------------
	proc RBC.barchart.configure.M.36.1.Setup {} {
		barchart .barchart1
		pack .barchart1
	}
	
	proc RBC.barchart.configure.M.36.1.Body {} {
		.barchart1 configure -width 200
	}
	
	proc RBC.barchart.configure.M.36.1.Cleanup {} {
		destroy .barchart1
	}
}
