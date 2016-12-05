library(lattice)  

data = read.table("E:\\data.csv", sep = ",");     # directory of the demand table
values = as.numeric(unlist(data[1]));             # read demand values
probs = as.numeric(unlist(data[2]));              # read probability

# routine: inventory
# input: minimum inventory - s, maximum inventory - S
# output: average month net reward
#================================== parameters =================================#
# invlev:		    dynamic inventory
# h:		        storage cost - h/item/month
# r:            retail price - r/item
# T:            simulation time
# setupc:       setup cost
# incrmc: 	    order cost - incremc/item
# d:		        total order cost = setup cost + order cost * inventory
# t:            current time
# t1:           next delivery arrival time
# H:            cumulative inventory cost
# C:            cumulative order cost
# R:            cumulative reward
# amount:       amount of order
# loss:         count of stockout
# p:            cost for stockout - p/item
# L:            delay time for delivery
# D:            demands 
# w:		        actual purchases
#===========================================================================#
inventory = function(s,S){  
  invlev = 60;
  h = 1;
  r = 10;
  T = 120;
  setupc = 32;
  incrmc = 3;
  d = function(invlev){setupc + incrmc * invlev} 
  t = 1; t1 = Inf; 
  H = 0; C = 0; R = 0;
  amount = 0; loss = 0; p = 5; budget = 200;
  
  while(t <= T) { 
    L = runif(1, min = 0.5, max = 1);           # random delay time for delivery
    H = H + h * invlev;                         # monthly inventory cost
    
    # if delivery arrives
    if(t1 <= t){  
      C = C + d(amount);                        # total order cost
      invlev = invlev + amount;  
      amount = 0;  
      t1 = Inf;                                 # set next deilivery time to infinity
    }  
    D = sample(values, 1, T, probs);            # random demands
    w = min(D, invlev)                          # actual purchases
    R = R + r * w;  
    
    # if demands above inventory
    if(D > invlev){  
      loss = loss + 1  
    }  
    invlev = invlev - w;  
     
    if(invlev < s && amount == 0){  
      amount = S - invlev;
     # if(d(amount) > budget){
     #   amount = floor((budget - setupc) / incrmc);
     #}
      t1 = t + L;                                 # change next delivery arrival time  
    }  
    t = t + 1;  
  }  
  return ((R - C - H - p * loss) / T)             # return average monthly net rewards
}  

# input bounds of the inventory
s1 = 30;
s2 = 50;
S1 = 70;
S2 = 100;
imax = (s2 - s1 + 1) * (S2 - S1 + 1);
s <- s1 : s2  
S <- S1 : S2  
data <- expand.grid(s, S)                         # combine s and S for plotting

# average 10 times for each combination (s, S)
for (i in 1 : imax) {  
  data$mean[i] = mean(replicate(10, inventory(data$Var1[i], data$Var2[i])))  
}  

# plot 2D heat map
levelplot(mean ~ Var1 * Var2, data = data,xlab = "minimum inventory",ylab = "maximum inventory")  

# list top 10 data combination
data[order(data$mean, decreasing = T)[1 : 10],]  

# output the best one, plot histogram for net rewards
first = data[order(data$mean, decreasing = T)[1 : 1],]  
result <- replicate(1000, inventory(first$Var1, first$Var2))  
hist(result)
