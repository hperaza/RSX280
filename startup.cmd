.enable substitution
.asks tim Please enter Time and Date (HR:MN DD-MMM-YY)
tim 'tim'
.ifins ...acs acs sy:/blks=512
set /colog=on
set /colog/nocoterm
pip [syslog]*.*/pu:5
upt
