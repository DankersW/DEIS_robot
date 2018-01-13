posData = [[-1,-1],[-1,-1],[-1,-1],[-1,-1],[-1,-1]] #-1 represents no data --> from the start no data has received
followingData = [-1,-1,-1,-1,-1] #-1 represents leader --> from start everyone is the leader
fanOutData = [-1,-1,-1,-1,-1] #-1 represents no data has received

def representsInt(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


#simulate receiving position data on the channel
print "Simulate receiving position data on the channel"
with open('posDataFile.txt', 'r') as f:
    for line in f:
        h = line.strip().split(',')
        if (len(h)==3 and representsInt(h[0]) and representsInt(h[1]) and representsInt(h[2])):
            posData[int(h[0])-1] = [int(h[1]), int(h[2])]
    print posData

#simulate receiving who is following who data on the channel
print "\nSimulate receiving who is following who data on the channel"
with open('followingDataFile.txt', 'r') as f:
    for line in f:
        h = line.strip().split(',')
        if (len(h)==2 and representsInt(h[0]) and representsInt(h[1])):
            followingData[int(h[0])-1] = int(h[1])
    print followingData

#simulate receiving fan out to a lane data
print "\nFan out to a lane data on channel"
with open('toGoToLaneDataFile.txt', 'r') as f:
    for line in f:
        corruptData = True
        h = line.strip().split(',')
        if(len(h) == 5):
            corruptData = False
            for i in range(len(h)):
                if not (representsInt(h[i])):
                    corruptData = True

        if not corruptData:
            toGoTo = h[2]
            for i in range(len(h)):
                fanOutData[i] = h[i]
    print fanOutData
    #toGoTo = fanOutData[2]
    print "Lane to go to: " + str(toGoTo)


#simulate receiving fan_out data on the channel
print "\nSimulate receiving fan_out data on the channel"
with open('fanOut.txt', 'r') as f:
    for line in f:
        h = line.strip()
        if (h == "fanOut"):
            fanOut = True

    print fanOut

print ""


def fanOutController(toGoToLane):
    '''

    '''
