import gym
import highway_env
import numpy as np
from stable_baselines3 import HerReplayBuffer, SAC, DDPG, TD3
from stable_baselines3.common.noise import NormalActionNoise


env = gym.make("parking-v0")
her_kwargs = dict(n_sampled_goal=4, goal_selection_strategy='future',
                  online_sampling=True, max_episode_length=100)
model = SAC(
    "MultiInputPolicy",
    env,
    replay_buffer_class=HerReplayBuffer,
    replay_buffer_kwargs=her_kwargs, verbose=1, buffer_size=int(1e6),
    learning_rate=1e-3,
    gamma=0.95, batch_size=1024, tau=0.05,
    policy_kwargs=dict(net_arch=[512, 512, 512])
)

model.learn(int(100000))
model.save("sac_highway-1")

model = SAC.load("sac_highway-1", env=env)

obs = env.reset()

episode_reward = 0
for _ in range(100000):
    action, _states = model.predict(obs, deterministic=True)
    obs, reward, done, info = env.step(action)
    env.render()
    episode_reward += reward
    if done or info.get("is_success", False):
        print("Reward:", episode_reward, "Success?",
              info.get("is_success", False))
        episode_reward = 0.0
        obs = env.reset()
