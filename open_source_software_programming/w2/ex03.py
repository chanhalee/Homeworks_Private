import gym
env = gym.make('CartPole-v1')
for i_episode in range(2000): # episode = 1회의 시뮬레이션, 여기선 20번의 에피소드
    observation = env.reset() # 매 에피소드 시작시 환경을 초기화한다.
    for t in range(100): # 1번의 에피소드는 100번의 스텝, (100 스텝 이전에 끝날 수도 있다.)
        env.render()
        print(observation)
        action = env.action_space.sample()
        observation, reward, done, info = env.step(action)
        print(reward)
        if done: # 에피소드가 끝날 경우 이 done 이 true가 되어 아래 명령문이 실행된다.
            print("Episode finished after {} timesteps".format(t+1))
            break
env.close()

# gym description
# https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py