import gym
import ale_py
from ale_py.roms import Breakout
from random import randrange
from ale_py import ALEInterface


env = gym.make("Taxi-v3")
for i_episode in range(100):
    observation = env.reset()  # 매 에피소드 시작시 환경을 초기화한다.
    for t in range(100):
        env.render()
        action = 1  # Your agent code here
        print(observation)
        observation, reward, done, info = env.step(action)
        if done:
            break
env.close()
