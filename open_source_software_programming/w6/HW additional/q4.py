import gym
import ale_py
from ale_py.roms import Breakout
from random import randrange
from ale_py import ALEInterface
from q1 import PolicyIteration


env = gym.make("Taxi-v3")
policy_iteration = PolicyIteration(env)
stable_theta = 0.01
discount_factor = 0.9
observation = env.reset()  # 매 에피소드 시작시 환경을 초기화한다.
# policy stabilize
stabilize_result = policy_iteration.policy_stable(
    discount_factor, stable_theta)
print("\n\n<theta=", stable_theta, " 대한 안정화 결과>\n 성공: ",
      stabilize_result[0][0], "\npolicy improvement 횟수: ", stabilize_result[0][1], "\npolicy evaluation 횟수: ", stabilize_result[0][2])
# print("\n=======value table=======\n")
# print(stabilize_result[1])
# print("\n=======policy table=======\n")
# print(stabilize_result[2])
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
