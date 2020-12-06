import numpy as np
# Rock 0; Paper 1; Scissor 2
def set_seed(id):
    np.random.seed(id)

def main():
    sigma_op = np.array([0.3,0.4,0.3]) # opponent mixed strategy

    sigma_my = np.array([1,0,0])

    regret_my = np.array([0,0,0])
    regret_op = np.array([0,0,0])

    uiiprime = np.array([[0,-1,1],[1,0,-1],[-1,1,0]]) #utility matrix
    # print(uiiprime)
    for i in range(100000):
    # should be in a loop
        s_prime = get_choice(sigma_op)
        s = get_choice(sigma_my)

        u_a = uiiprime[s,s_prime]

        # update for me
        for j in range(3):
            regret_my[j] += uiiprime[j,s_prime] - u_a
        regret_my[regret_my<0]=0

        # update for opponent
        for j in range(3):
            regret_op[j] += - ( uiiprime[s,j] - u_a )
        regret_op[regret_op<0]=0

        # regret update
        sigma_my = regret_my/np.sum(regret_my)
        # sigma_op = regret_op/np.sum(regret_op)
        # print("my "+str(sigma_my))
        # print("op "+str(sigma_op))
    print("my "+str(sigma_my))
    print("op "+str(sigma_op))

def get_choice(sigma):
    ran = np.random.uniform()
    if ran < sigma[0]:
        s = 0
    elif ran < sigma[0]+sigma[1]:
        s = 1
    else:
        s = 2
    return s
    
if __name__ == "__main__":
    set_seed(id=42)
    main()






