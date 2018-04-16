# All individual node infromation shall be in CSV formatted as "hostname,mp,cp
#   hostname - Name of host where node is located
#   mp - Main (command) port of that node. Also used in formatting unique
#       path to file-server root dir. Peers talk amongst each other on
#       this port.
#   cp - Command port. Clients (non-peers) send messages to the server on
#       this port to indicate vairous commands. These are detailed in the
#       client program.
#
# The tracking server information shall be listed under name N0.
# The tracking server shall also have a cp of 0.
# Each node's respective environment variable name shall be formatted as
#   an 'N' followed by a non-negative integer.
#
# All node relationships shall be in CSV formatted as lhs,rhs,latency.
#   lhs and rhs are names of nodes. The number of lhs shall be less than rhs.
#   No transitive relationships shall be allowed (eg. n0,n0,1000)
# Each environment variable name indicating a relationship shall begin
#   with "REL", and end in a non-negative integer.
#

export FS_ROOT=~/5105_peer_files

export N0=hostname,0,8990
export N1=hostname,8000,8080
export N2=hostname,8990,8999

export REL0=N1,N2,0
