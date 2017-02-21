#
# Copyright (C) 2017 Ryan C. Scott <ryan@5pmCasual.com>
#

#!/usr/bin/env python

import os
import sys
import subprocess
import re
import argparse

def ChangePaths( args ):
    for path in args.files:
        proc = subprocess.Popen( [ 'otool -L %s' % path ], stdout=subprocess.PIPE, shell=True )
        ( out, err ) = proc.communicate()

        for line in out.split( '\n' ):
            matches = re.search( '%s(.*?)\s\(' % args.search, line )

            if matches:
                libName = matches.group( 1 )
                cmd = 'install_name_tool -change %s%s @executable_path/%s%s %s' % ( args.search, libName, args.replace, libName, path )

                print cmd
                os.system( cmd )

    
#####
parser = argparse.ArgumentParser( description="" )
parser.add_argument( '-s', '--search', default='/usr/local/lib/', help='' )
parser.add_argument( '-r', '--replace', default='', help='' )
parser.add_argument( 'files', metavar='files', nargs='+', help='Binary to search/change' )
args = parser.parse_args()

#####
ChangePaths( args )
