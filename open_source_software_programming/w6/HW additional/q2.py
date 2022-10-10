import gym
import ale_py
from ale_py.roms import Breakout
from random import randrange
from ale_py import ALEInterface
from q1 import PolicyIteration


env = gym.make("Taxi-v3")
policy_iteration = PolicyIteration(env)
observation = env.reset()  # 매 에피소드 시작시 환경을 초기화한다.
for a in range(10):
    policy_iteration.policy_evaluation(10, 0.95)
    policy_iteration.policy_improvement()
# print(policy_iteration.get_values())
done = False
for a in range(50):
    env.render()
    state = list(env.decode(observation))
    action = policy_iteration.get_action(state)  # Your agent code here
    observation, reward, done, info = env.step(action)
    if done:
        print("진행 횟수: ", a)
        if reward == 20:
            print("성공 여부: 성공")
        else:
            print("성공 여부: 실패")
        break
env.close()
