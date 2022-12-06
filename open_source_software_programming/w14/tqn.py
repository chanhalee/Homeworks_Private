import gym
import highway_env
from stable_baselines3 import HerReplayBuffer, SAC
from sb3_contrib import TQC
from time import sleep

env = gym.make("parking-v0")
env.reset()
model = TQC.load("model/tqc_parking", env=env)
for i_episode in range(10):
    obs = env.reset()
    while True:
        sleep(0.1)
        env.render()
        action, _ = model.predict(obs, deterministic=True)
        # print(action)
        obs, reward, done, info = env.step(action)
        if done:
            break

env.close()
