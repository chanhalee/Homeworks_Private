# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html


#
from __future__ import unicode_literals
from imp import reload

from scrapy.exporters import JsonItemExporter, CsvItemExporter

class MoviecrawlerPipeline(object):
    def __init__(self):
        self.file = open("MovieCrawl.csv", 'wb')
        self.exporter = CsvItemExporter(self.file, encoding='utf-8')
        self.exporter.start_exporting()
 
    def process_item(self, item, spider):
        self.exporter.export_item(item)
        return item
         
    def spider_closed(self, spider):
        self.exporter.finish_exporting()
        self.file.close()   #파일 CLOSE 
