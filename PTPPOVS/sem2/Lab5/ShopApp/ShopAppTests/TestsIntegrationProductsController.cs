using ShopApp.Services;
using ShopApp.Models;
using NUnit.Framework;
using System;
using System.IO;
using ShopApp.WebApi.DB;
using Microsoft.Data.Sqlite;

namespace ShopAppTests.Controllers
{
    [TestFixture]
    public class TestsIntegrationProductsController
    {
        private ProductsController _controller;
        private IProductService _productService;
        private string _testDbPath = "test_db.db";

        [SetUp]
        public void Setup()
        {
            CleanupDatabase();

            var db = new DataBase($"Data Source={_testDbPath};");
            _productService = new ProductService(db);
            _controller = new ProductsController(_productService);
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

        // Tests for CreateProduct
        [TestCase("Valid Product", 10.0)]
        [TestCase("", 15.0)]
        [TestCase("Valid Product", -5.0)]
        [TestCase("   ", 25.0)]
        public void CreateProduct_ShouldReturnProductAndPersist_WhenInputIsValid(string description, double price)
        {
            var result = _controller.CreateProduct(description, price);

            if (string.IsNullOrWhiteSpace(description) || price < 0)
            {
                Assert.That(result, Is.Null, "Should reject invalid input");
            }
            else
            {
                Assert.Multiple(() =>
                {
                    Assert.That(result, Is.Not.Null, "Created product should not be null");
                    Assert.That(result.Description, Is.EqualTo(description), "Product description mismatch");
                    Assert.That(result.Price, Is.EqualTo(price), "Product price mismatch");

                    var fromDb = _productService.Search(result.Id);
                    Assert.That(fromDb, Is.Not.Null, "Product not found in database");
                    Assert.That(fromDb.Description, Is.EqualTo(description), "Stored description mismatch");
                    Assert.That(fromDb.Price, Is.EqualTo(price), "Stored price mismatch");
                });
            }
        }

        // Tests for SearchProduct
        [Test]
        public void SearchProduct_ShouldReturnProduct_WhenProductExists()
        {
            var product = _controller.CreateProduct("Searchable", 15.99);
            var result = _controller.SearchProduct(product.Id);

            Assert.Multiple(() =>
            {
                Assert.That(result, Is.Not.Null, "Product not found");
                Assert.That(result.Id, Is.EqualTo(product.Id), "Product ID mismatch");
                Assert.That(result.Description, Is.EqualTo(product.Description), "Description mismatch");
                Assert.That(result.Price, Is.EqualTo(product.Price), "Price mismatch");
            });
        }

        [Test]
        public void SearchProduct_ShouldReturnNull_WhenProductDoesNotExist()
        {
            var result = _controller.SearchProduct(Guid.NewGuid());
            Assert.That(result, Is.Null);
        }

        // Tests for RemoveProduct
        [Test]
        public void RemoveProduct_ShouldReturnProductAndDelete_WhenProductExists()
        {
            var product = _controller.CreateProduct("To Delete", 9.99);
            var removed = _controller.RemoveProduct(product.Id);

            Assert.Multiple(() =>
            {
                Assert.That(removed, Is.Not.Null, "Null returned for removed product");
                Assert.That(removed.Id, Is.EqualTo(product.Id), "Removed product ID mismatch");

                var postRemovalSearch = _productService.Search(product.Id);
                Assert.That(postRemovalSearch, Is.Null, "Product still exists after removal");
            });
        }

        [Test]
        public void RemoveProduct_ShouldReturnNull_WhenProductDoesNotExist()
        {
            var result = _controller.RemoveProduct(Guid.NewGuid());
            Assert.That(result, Is.Null);
        }

        // Tests for EditProduct
        [TestCase("Updated Product", 20.0)]
        [TestCase("", 30.0)]
        [TestCase("Valid Update", -10.0)]
        public void EditProduct_ShouldReturnUpdatedProduct_WhenInputIsValid(string newDescription, double newPrice)
        {
            var original = _controller.CreateProduct("Original", 10.0);
            var result = _controller.EditProduct(original.Id, newDescription, newPrice);

            if (string.IsNullOrWhiteSpace(newDescription) || newPrice < 0)
            {
                Assert.Multiple(() =>
                {
                    Assert.That(result, Is.Null, "Should reject invalid update");
                    var fromDb = _productService.Search(original.Id);
                    Assert.That(fromDb.Description, Is.EqualTo(original.Description), "Description should not change");
                    Assert.That(fromDb.Price, Is.EqualTo(original.Price), "Price should not change");
                });
            }
            else
            {
                Assert.Multiple(() =>
                {
                    Assert.That(result, Is.Not.Null, "Should accept valid update");
                    Assert.That(result.Description, Is.EqualTo(newDescription));
                    Assert.That(result.Price, Is.EqualTo(newPrice));

                    var fromDb = _productService.Search(original.Id);
                    Assert.That(fromDb.Description, Is.EqualTo(newDescription), "Database description not updated");
                    Assert.That(fromDb.Price, Is.EqualTo(newPrice), "Database price not updated");
                });
            }
        }
    }
}
