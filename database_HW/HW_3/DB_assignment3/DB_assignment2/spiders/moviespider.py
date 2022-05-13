
from imp import reload
from pymysql import NULL
import scrapy

from scrapy.spiders import Spider

from DB_assignment2.items import MovieCrawlerItem

from scrapy.http import Request

from scrapy.selector import Selector

class MoviespiderSpider(scrapy.Spider):
	name = 'moviespider'
	allowed_domains = ['movie.naver.com']
	
	def start_requests(self):
		yield scrapy.Request("https://movie.naver.com/movie/running/current.naver",self.parse)

	def parse(self, response):
		for colum in response.xpath('//div[@id="wrap"]/div[@id="container"]/div[@id="content"]/div[@class="article"]/div[1]/div[@class="lst_wrap"]/ul/li'):
			item = MovieCrawlerItem()
			item['title'] = colum.xpath('dl/dt[@class="tit"]/a/text()').extract_first()
			item['movie_rate'] = colum.xpath('dl/dt[@class="tit"]/span[1]/text()').extract_first()
			temp = NULL
			temp = str(colum.xpath('dl/dd[@class="star"]/dl[@class="info_star"]/dd[1]/div[@class="star_t1"]/a[1]/span[@class="num"]/text()').extract_first())
			if(temp != 'None'):
				item['netizen_rate'] = float(temp)
			temp = str(colum.xpath('dl/dd[@class="star"]/dl[@class="info_star"]/dd[1]/div[@class="star_t1"]/a[1]/span[@class="num2"]/em/text()').extract_first())
			if(temp != 'None'):
				item['netizen_count'] = int(temp.replace(",", ""))
			temp = str(colum.xpath('dl/dd[@class="star"]/dl[@class="info_star"]/dd[2]/div[@class="star_t1"]/a[1]/span[@class="num"]/text()').extract_first())
			if(temp != 'None'):
				item['journalist_score'] = float(temp)
			temp = str(colum.xpath('dl/dd[@class="star"]/dl[@class="info_star"]/dd[2]/div[@class="star_t1"]/a[1]/span[@class="num2"]/em/text()').extract_first())
			if(temp != 'None'):
				item['journalist_count'] = int(temp.replace(",", ""))
			genreStr = []
			for genre in colum.xpath('dl/dd[2]/dl[@class="info_txt1"]/dd[1]/span[@class="link_txt"]/a'):
				genreStr.append(str(genre.xpath('text()').extract_first()))
			item['scope'] = ','.join(genreStr)
			item['playing_time'] = str(colum.xpath('dl/dd[2]/dl[@class="info_txt1"]/dd[1]/text()[3]').extract_first()).strip().replace("분","")
			item['opening_date'] = str(colum.xpath('dl/dd[2]/dl[@class="info_txt1"]/dd[1]/text()[4]').extract_first()).strip().replace(" 개봉","").replace(".","-")
			dirStr = []
			for dir in colum.xpath('dl/dd[2]/dl[@class="info_txt1"]/dd[2]/span[@class="link_txt"]/a'):
				dirStr.append(str(dir.xpath('text()').extract_first()))
			item['director'] = ','.join(dirStr)
			item['image'] = colum.xpath('div[@class="thumb"]/a[1]/img/@src').extract_first()
			yield item