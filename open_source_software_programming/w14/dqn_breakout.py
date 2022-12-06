import gym
import torch
from stable_baselines3 import DQN

env = gym.make('ALE/Breakout-v5')

model = DQN('CnnPolicy', env, learning_rate=5e-4,
            buffer_size=15000,
            learning_starts=200,
            batch_size=32,
            gamma=0.8,
            train_freq=1,
            gradient_steps=1,
            target_update_interval=50,
            verbose=1,
            tensorboard_log="logs/dqn_cnn/")

model.learn(total_timesteps=3000)
model.save("dqn_atari")
env.close()
env = gym.make('ALE/Breakout-v5', render_mode='human')

for i_episode in range(5):
    obs = env.reset()
    for t in range(1000):
        action, _state = model.predict(obs)
        # print(action)
        obs, reward, done, info = env.step(action)
        if done:
            print("Episode finished after {} timesteps".format(t+1))
            break
env.close()
