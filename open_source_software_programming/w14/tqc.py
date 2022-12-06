import gym
import highway_env
from stable_baselines3 import HerReplayBuffer, SAC
from sb3_contrib import TQC

env = gym.make("parking-v0")
her_kwargs = dict(n_sampled_goal=4, goal_selection_strategy='future', online_sampling=True, max_episode_length=100)
# You can replace TQC with SAC agent
model = TQC('MultiInputPolicy', env, replay_buffer_class=HerReplayBuffer,
            replay_buffer_kwargs=her_kwargs, verbose=1, buffer_size=int(1e6),
            learning_rate=1e-3,
            gamma=0.95, batch_size=1024, tau=0.05,
            policy_kwargs=dict(net_arch=[512, 512, 512]),
            tensorboard_log="logs/tqc_cnn/")
model.learn(int(5e4))

model.save("model/tqc_parking2")

env = gym.make("parking-v0")
for i_episode in range(5):
    obs, done = env.reset()
    while True:
        env.render()
        action, _ = model.predict(obs, deterministic=True)
        #print(action)
        obs, reward, done, info = env.step(action)
        if done:
            #print("Episode finished after {} timesteps".format(t+1))
            break
env.close()