using ShopApp.Services;
using ShopApp.Models;
using NUnit.Framework;
using System;
using System.IO;
using ShopApp.WebApi.DB;
using Microsoft.Data.Sqlite;

namespace ShopAppTests.Services
{
    [TestFixture]
    internal class TestsIntegrationProductService
    {
        private IProductService _productService;
        private Product _testProduct;
        private string _testDbPath = "test_db.db";

        [SetUp]
        public void Setup()
        {
            CleanupDatabase();

            var db = new DataBase($"Data Source={_testDbPath};");
            _productService = new ProductService(db);

            _testProduct = new Product
            {
                Id = Guid.NewGuid(),
                Description = "Test Product",
                Price = 10.0
            };
        }

        [TearDown]
        public void Teardown()
        {
            CleanupDatabase();
        }

        private void CleanupDatabase()
        {
            SqliteConnection.ClearAllPools();

            if (File.Exists(_testDbPath))
            {
                File.Delete(_testDbPath);
            }
        }

        // Add tests
        [TestCase("Product Description", 10)]
        [TestCase("", 15)]
        [TestCase("Product Description", 10.15)]
        public void TestAddMethod_ShouldAddProduct_WhenNew(string description, double price)
        {
            var product = new Product
            {
                Id = Guid.NewGuid(),
                Description = description,
                Price = price
            };

            var result = _productService.Add(product);
            Assert.That(result, Is.EqualTo(product), "Product was not added correctly");
        }

        [Test]
        public void TestAddMethod_ShouldReturnNull_WhenDuplicate()
        {
            _productService.Add(_testProduct);
            var result = _productService.Add(_testProduct);
            Assert.That(result, Is.Null, "A duplicate product has been added.");
        }

        // Remove tests
        [Test]
        public void TestRemoveMethod_ShouldRemoveProduct_WhenExists()
        {
            _productService.Add(_testProduct);
            var result = _productService.Remove(_testProduct.Id);

            Assert.Multiple(() =>
            {
                Assert.That(result.Id, Is.EqualTo(_testProduct.Id));
                Assert.That(result.Description, Is.EqualTo(_testProduct.Description));
                Assert.That(result.Price, Is.EqualTo(_testProduct.Price));
                Assert.That(_productService.Search(_testProduct.Id), Is.Null);
            });
        }

        [Test]
        public void TestRemoveMethod_ShouldReturnNull_WhenNotExists()
        {
            var result = _productService.Remove(Guid.NewGuid());
            Assert.That(result, Is.Null);
        }

        // Edit tests
        [Test]
        public void TestEditMethod_ShouldEditProduct_WhenExists()
        {
            _productService.Add(_testProduct);

            var updatedProduct = new Product
            {
                Id = _testProduct.Id,
                Description = "Updated Product",
                Price = 30.0
            };

            var result = _productService.Edit(updatedProduct);

            Assert.Multiple(() =>
            {
                Assert.That(result, Is.EqualTo(updatedProduct));
                var searched = _productService.Search(updatedProduct.Id);
                Assert.That(searched?.Description, Is.EqualTo(updatedProduct.Description));
                Assert.That(searched?.Price, Is.EqualTo(updatedProduct.Price));
            });
        }

        [Test]
        public void TestEditMethod_ShouldReturnNull_WhenNotExists()
        {
            var result = _productService.Edit(_testProduct);
            Assert.That(result, Is.Null);
        }

        // Search tests
        [Test]
        public void TestSearchMethod_ShouldFindProduct_WhenExists()
        {
            _productService.Add(_testProduct);
            var result = _productService.Search(_testProduct.Id);

            Assert.Multiple(() =>
            {
                Assert.That(result?.Id, Is.EqualTo(_testProduct.Id));
                Assert.That(result?.Description, Is.EqualTo(_testProduct.Description));
                Assert.That(result?.Price, Is.EqualTo(_testProduct.Price));
            });
        }

        [Test]
        public void TestSearchMethod_ShouldReturnNull_WhenNotExists()
        {
            var result = _productService.Search(Guid.NewGuid());
            Assert.That(result, Is.Null);
        }
    }
}
