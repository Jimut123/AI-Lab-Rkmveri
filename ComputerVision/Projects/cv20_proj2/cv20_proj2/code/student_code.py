import numpy as np
import cv2 # You must not use any methods which has 'hough' in it!
from utils import  hough_peaks


def hough_lines_vote_acc(edge_img, rho_res=1, thetas= np.arange(0,180)):
    """
      Creating an Hough Vote Accumulator. The Generated grid will have thetas on 
      X axis, and rhos on the Y axis. 

      Args
      - edge_img: numpy nd-array of dim (m, n)
      - rho_res: rho resolution. Default is 1. This controls how dense your grid columns are
      - thetas: theta values. 
        
      Returns
      - accumulator with votes for each grid point, thetas, and rhos

      HINTS:
      - I encourage you to try implementing this naively first, just be aware that
       it may take an absurdly long time to run. You will need to get a function
       that takes a reasonable amount of time to run so that I can verify
       your code works.
    """
    ############################
    ### TODO: YOUR CODE HERE ###


    # x*cos(theta) + y*sin(theta) max value sin,cos theta can take is 1
    max_rad = edge_img.shape[0] + edge_img.shape[1]
    accumulator = [[0 for x in range(180)] for y in range(max_rad)]  #np.zeros((max_rad,max_rad))
    #print(accumulator)
    for x in range(edge_img.shape[0]):
        for y in range(edge_img.shape[1]):
            if edge_img[x][y] == 255:
                for theta in thetas:
                    # convert it into radians multiply by (np.pi/180)
                    rho = x*np.cos(theta*np.pi/180)+y*np.sin(theta*np.pi/180)
                    #print(rho)
                    #print("Type = ",rho," ",theta)
                    accumulator[int(rho.astype(int))][theta] += 1
    rhos = [x for x in range(max_rad)]
    accumulator = np.asarray(accumulator, dtype=np.uint8)


    ### END OF STUDENT CODE ####
    ############################
    return accumulator, thetas, rhos

    
def hough_circles_vote_acc(edge_img, radius):
    """
      Creating an Hough Vote Accumulator. The Generated grid will have 
      x coordinate of the center of cirlce on 
      X axis, and y coordinate of the center of cirlces on the Y axis. 

      Args
      - edge_img: numpy nd-array of dim (m, n)
      - radius: radius of the circle
        
      Returns
      - accumulator with votes for each grid point

      HINTS:
      - I encourage you to try implementing this naively first, just be aware that
       it may take an absurdly long time to run. You will need to get a function
       that takes a reasonable amount of time to run so that I can verify
       your code works.
    """
    ############################
    ### TODO: YOUR CODE HERE ###

    # Naive version of the accumulator needs 3 dimensional matrix
    # Hough Gradient Method will give faster results

    accumulator = [[0 for x in range(edge_img.shape[1])] for y in range(edge_img.shape[0])]
    for x in range(edge_img.shape[0]):
        for y in range(edge_img.shape[1]):
            if edge_img[x][y] == 255:
                    for theta in range(0,360):
                        b = x - radius * np.cos(theta*np.pi/180)    # x – r * cos(t * PI / 180);
                        a = y - radius * np.sin(theta*np.pi/180)    # y – r * sin(t * PI / 180);
                        try:
                            # wild cards!
                            accumulator[int(abs(b))][int(abs(a))] += 1 
                        except:
                            pass
    accumulator = np.array(accumulator, dtype=np.uint8)


    ### END OF STUDENT CODE ####
    ############################        
    return accumulator



def find_circles(edge_img, radius_range=[1,2], threshold=100, nhood_size=10):
    """
      A naive implementation of the algorithm for finding all the circles in a range.
      Feel free to write your own more efficient method [Extra Credit]. 
      For extra credit, you may need to add additional arguments. 


      Args
      - edge_img: numpy nd-array of dim (m, n). 
      - radius_range: range of radius. All cicles whose radius falls 
      in between should be selected.
      - nhood_size: size of the neighborhood from where only one candidate can be chosen. 
      
      Returns
      - centers, and radii i.e., (x, y) coordinates for each circle.

      HINTS:
      - I encourage you to use this naive version first. Just be aware that
       it may take a long time to run. You will get EXTRA CREDIT if you can write a faster
       implementaiton of this method, keeping the method signature (input and output parameters)
       unchanged. 
    """
    n = radius_range[1] - radius_range[0]
    H_size = (n,) + edge_img.shape
    H = np.zeros(H_size, dtype=np.uint)
    centers = ()
    radii = np.arange(radius_range[0], radius_range[1])
    valid_radii = np.array([], dtype=np.uint)
    num_circles = 0
    for i in range(len(radii)):
        H[i] = hough_circles_vote_acc(edge_img, radii[i])
        peaks = hough_peaks(H[i], numpeaks=10, threshold=threshold,
                            nhood_size=nhood_size)
        if peaks.shape[0]:
            valid_radii = np.append(valid_radii, radii[i])
            centers = centers + (peaks,)
            for peak in peaks:
                cv2.circle(edge_img, tuple(peak[::-1]), radii[i]+1, (0,0,0), -1)
        #  cv2.imshow('image', edge_img); cv2.waitKey(0); cv2.destroyAllWindows()
        num_circles += peaks.shape[0]
        print('Progress: %d%% - Circles: %d\033[F\r'%(100*i/len(radii), num_circles))
    print('Circles detected: %d          '%(num_circles))
    centers = np.array(centers)
    return centers, valid_radii.astype(np.uint)
