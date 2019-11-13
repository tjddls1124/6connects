import gym;
import numpy as np;
import matplotlib.pyplot as plt;
from gym.envs.registration import register


register(
    id = '6connect',
    entry_point='gym.envs.toy_text:FrozenLakeEnv',
    kwargs={'map_name' : '4x4' ,
            'is_slipery' : False}
)
env = gym.make('6connect')

#Initailize
Q = np.zeros([env.observation_space.n,env.action_space.n]);
dis=.99
num_episodes = 2000
rList = []

