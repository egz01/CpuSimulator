import os


path = r"C:\Users\eviat\Documents\university\Semester 5\Computer Organization\source\CompOrgProj\test_programs\disktest\diskin.txt"
sectors = []
# generate disk patterns
for i in range(8):
    sectors.append([i for j in range(128)])

#flatten to one array
disk = []
for sector in sectors:
    disk += sector
    

