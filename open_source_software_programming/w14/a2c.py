import gym
import highway_env
import numpy as np
import torch
from stable_baselines3 import A2C

env = gym.make("parking-v0")

model = A2C('MultiInputPolicy', env,
            verbose=1,
            learning_rate=1e-3,
            gamma=0.95,)
model.learn(total_timesteps=30000)

obs = env.reset()
for i in range(100000):
    env.render()
    action, _state = model.predict(obs, deterministic=True)
    obs, reward, done, info = env.step(action)
    if done:
        obs = env.reset()
